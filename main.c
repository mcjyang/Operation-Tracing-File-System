/*
 * Copyright (c) 1998-2015 Erez Zadok
 * Copyright (c) 2009	   Shrikar Archak
 * Copyright (c) 2003-2015 Stony Brook University
 * Copyright (c) 2003-2015 The Research Foundation of SUNY
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/*
 * CSE-506 : Operating Systems HW2
 * Author  : Meng-Chieh Yang
 * ID      : 110452591
 * Date    : 11/1/16
 *
 * main.c
 * - Modify trfs_read_super for modified type of raw data.
 * - Modify trfs_mount for accepting mounting option.
 */


#include <linux/module.h>
#include "trfs.h"

spinlock_t sl;
/*
 * There is no need to lock the trfs_super_info's rwsem as there is no
 * way anyone can have a reference to the superblock at this point in time.
 */
static int trfs_read_super(struct super_block *sb, void *raw_data, int silent)
{
	int err = 0;
	struct super_block *lower_sb;
	struct path lower_path;
	struct inode *inode;
	struct trfs_dev_w_opt *bundle;
	char *dev_name;
	char *out_file;
	
	bundle = (struct trfs_dev_w_opt *) raw_data;
	dev_name = (char *) bundle->lower_path_name;
	out_file = bundle->out_file;

	spin_lock_init(&sl);

	if (!dev_name) {
		printk(KERN_ERR
		       "trfs: read_super: missing dev_name argument\n");
		err = -EINVAL;
		goto out;
	}

	/* parse lower path */
	err = kern_path(dev_name, LOOKUP_FOLLOW | LOOKUP_DIRECTORY,
			&lower_path);
	if (err) {
		printk(KERN_ERR	"trfs: error accessing "
		       "lower directory '%s'\n", dev_name);
		goto out;
	}

	/* allocate superblock private data */
	sb->s_fs_info = kzalloc(sizeof(struct trfs_sb_info), GFP_KERNEL);
	if (!TRFS_SB(sb)) {
		printk(KERN_CRIT "trfs: read_super: out of memory\n");
		err = -ENOMEM;
		goto out_free;
	}

	/* set the lower superblock field of upper superblock */
	lower_sb = lower_path.dentry->d_sb;
	atomic_inc(&lower_sb->s_active);

	/* modified here */
	trfs_set_lower_super(sb, lower_sb, out_file, 0, 0xffffffff);

	/* inherit maxbytes from lower file system */
	sb->s_maxbytes = lower_sb->s_maxbytes;

	/*
	 * Our c/m/atime granularity is 1 ns because we may stack on file
	 * systems whose granularity is as good.
	 */
	sb->s_time_gran = 1;

	sb->s_op = &trfs_sops;

	sb->s_export_op = &trfs_export_ops; /* adding NFS support */

	/* get a new inode and allocate our root dentry */
	inode = trfs_iget(sb, d_inode(lower_path.dentry));
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto out_sput;
	}
	sb->s_root = d_make_root(inode);
	if (!sb->s_root) {
		err = -ENOMEM;
		goto out_iput;
	}
	d_set_d_op(sb->s_root, &trfs_dops);

	/* link the upper and lower dentries */
	sb->s_root->d_fsdata = NULL;
	err = new_dentry_private_data(sb->s_root);
	if (err)
		goto out_freeroot;

	/* if get here: cannot have error */

	/* set the lower dentries for s_root */
	trfs_set_lower_path(sb->s_root, &lower_path);

	/*
	 * No need to call interpose because we already have a positive
	 * dentry, which was instantiated by d_make_root.  Just need to
	 * d_rehash it.
	 */
	d_rehash(sb->s_root);
	if (!silent)
		printk(KERN_INFO
		       "trfs: mounted on top of %s type %s\n",
		       dev_name, lower_sb->s_type->name);
	goto out; /* all is well */

	/* no longer needed: free_dentry_private_data(sb->s_root); */
out_freeroot:
	dput(sb->s_root);
out_iput:
	iput(inode);
out_sput:
	/* drop refs we took earlier */
	atomic_dec(&lower_sb->s_active);
	kfree(TRFS_SB(sb));
	sb->s_fs_info = NULL;
out_free:
	path_put(&lower_path);

out:
	return err;
}

struct dentry *trfs_mount(struct file_system_type *fs_type, int flags,
			    const char *dev_name, void *raw_data)
{
	
	struct trfs_dev_w_opt *bundle;
	char *prefix = NULL;
	char *out_file;
	struct dentry *d;


	prefix = strsep((char **)&raw_data, "=");
	if(strcmp(prefix,"tfile")!=0){
		printk("mount fail: bad option, please follow format: '-o tfile=/path/name'\n");
		return ERR_PTR(-1);
	}

	strcat((char *)raw_data, "\0");

	/* allocate memory to pack lower path name and outfile name*/
	bundle = (struct trfs_dev_w_opt *) kzalloc(sizeof(struct trfs_dev_w_opt), GFP_KERNEL);
	if(!bundle)
		return ERR_PTR(-ENOMEM);

	out_file = (char *) kzalloc(strlen(raw_data)*sizeof(char), GFP_KERNEL);
	if(!out_file){
		kfree(bundle);
		return ERR_PTR(-ENOMEM);
	}

	strcpy(out_file, raw_data);

	/* modified here */
	bundle->lower_path_name = dev_name;
	bundle->out_file = out_file;

	d = mount_nodev(fs_type, flags, (void *) bundle,
			   trfs_read_super);
	
	if(IS_ERR(d)){
		if(bundle->out_file)
			kfree(bundle->out_file);
		if(bundle)
			kfree(bundle);
	}

	return d;
}



static struct file_system_type trfs_fs_type = {
	.owner		= THIS_MODULE,
	.name		= TRFS_NAME,
	.mount		= trfs_mount,
	.kill_sb	= generic_shutdown_super,
	.fs_flags	= 0,
};
MODULE_ALIAS_FS(TRFS_NAME);

static int __init init_trfs_fs(void)
{
	int err;

	pr_info("Registering trfs " TRFS_VERSION "\n");

	err = trfs_init_inode_cache();
	if (err)
		goto out;
	err = trfs_init_dentry_cache();
	if (err)
		goto out;
	err = register_filesystem(&trfs_fs_type);
out:
	if (err) {
		trfs_destroy_inode_cache();
		trfs_destroy_dentry_cache();
	}
	return err;
}

static void __exit exit_trfs_fs(void)
{
	trfs_destroy_inode_cache();
	trfs_destroy_dentry_cache();
	unregister_filesystem(&trfs_fs_type);
	pr_info("Completed trfs module unload\n");
}

MODULE_AUTHOR("Erez Zadok, Filesystems and Storage Lab, Stony Brook University"
	      " (http://www.fsl.cs.sunysb.edu/)");
MODULE_DESCRIPTION("trfs " TRFS_VERSION
		   " (http://trfs.filesystems.org/)");
MODULE_LICENSE("GPL");

module_init(init_trfs_fs);
module_exit(exit_trfs_fs);
