/*
 *  linux/fs/char_dev.c
 *
 *  (C) 1991  Linus Torvalds
 */

#include <errno.h>
#include <sys/types.h>

#include <linux/sched.h>
#include <linux/kernel.h>

#include <asm/segment.h>
#include <asm/io.h>
#include <string.h>
#include <stdarg.h>

extern int tty_read(unsigned minor,char * buf,int count);
extern int tty_write(unsigned minor,char * buf,int count);

typedef int (*crw_ptr)(int rw,unsigned minor,char * buf,int count,off_t * pos);

static int rw_ttyx(int rw,unsigned minor,char * buf,int count,off_t * pos)
{
	return ((rw==READ)?tty_read(minor,buf,count):
		tty_write(minor,buf,count));
}

static int rw_tty(int rw,unsigned minor,char * buf,int count, off_t * pos)
{
	if (current->tty<0)
		return -EPERM;
	return rw_ttyx(rw,current->tty,buf,count,pos);
}

static int rw_ram(int rw,char * buf, int count, off_t *pos)
{
	return -EIO;
}

static int rw_mem(int rw,char * buf, int count, off_t * pos)
{
	return -EIO;
}

static int rw_kmem(int rw,char * buf, int count, off_t * pos)
{
	return -EIO;
}

static int rw_port(int rw,char * buf, int count, off_t * pos)
{
	int i=*pos;

	while (count-->0 && i<65536) {
		if (rw==READ)
			put_fs_byte(inb(i),buf++);
		else
			outb(get_fs_byte(buf++),i);
		i++;
	}
	i -= *pos;
	*pos += i;
	return i;
}

static int rw_memory(int rw, unsigned minor, char * buf, int count, off_t * pos)
{
	switch(minor) {
		case 0:
			return rw_ram(rw,buf,count,pos);
		case 1:
			return rw_mem(rw,buf,count,pos);
		case 2:
			return rw_kmem(rw,buf,count,pos);
		case 3:
			return (rw==READ)?0:count;	/* rw_null */
		case 4:
			return rw_port(rw,buf,count,pos);
		default:
			return -EIO;
	}
}

#define NRDEVS ((sizeof (crw_table))/(sizeof (crw_ptr)))

static crw_ptr crw_table[]={
	NULL,		/* nodev */
	rw_memory,	/* /dev/mem etc */
	NULL,		/* /dev/fd */
	NULL,		/* /dev/hd */
	rw_ttyx,	/* /dev/ttyx */
	rw_tty,		/* /dev/tty */
	NULL,		/* /dev/lp */
	NULL};		/* unnamed pipes */

int rw_char(int rw,int dev, char * buf, int count, off_t * pos)
{
	crw_ptr call_addr;

	if (MAJOR(dev)>=NRDEVS)
		return -ENODEV;
	if (!(call_addr=crw_table[MAJOR(dev)]))
		return -ENODEV;
	return call_addr(rw,MINOR(dev),buf,count,pos);
}

static int sprintf(char *buf, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsprintf(buf, fmt, args);
	va_end(args);
	return i;
}


void hd_infos(char * str) {
	struct super_block * sb = get_super(current -> root -> i_dev);	
	struct buffer_head * bh;
	unsigned short total_blocks = sb->s_nzones;	
	unsigned short s_imap_blocks = sb -> s_imap_blocks;
	unsigned short s_zmap_blocks = sb -> s_zmap_blocks;
	unsigned short used_blocks = 0;
	int i;
	for(i=0; s_zmap_blocks; i++)
	{
		bh = sb->s_zmap[i];
		char * p=(char *)bh->b_data;
		while(*p != '\0') {
			if ((*p) & 0x01) {
				used_blocks++;
			}
			if ((*p) & 0x02) {
				used_blocks++;
			}
			if ((*p) & 0x04) {
				used_blocks++;
			}
			if ((*p) & 0x02) {
				used_blocks++;
			}
			if ((*p) & 0x08) {
				used_blocks++;
			}
			if ((*p) & 0x10) {
				used_blocks++;
			}
			if ((*p) & 0x20) {
				used_blocks++;
			}
			if ((*p) & 0x40) {
				used_blocks++;
			}
			if ((*p) & 0x80) {
				used_blocks++;
			}
			p++;
		}
	}
	unsigned short free_blocks = total_blocks - used_blocks;
	int total_inodes = 0;
	for(i=0; s_imap_blocks; i++)
	{
		bh = sb->s_imap[i];
		char * p=(char *)bh->b_data;
		while(*p != '\0') {
			if ((*p) & 0x01) {
				total_inodes++;
			}
			if ((*p) & 0x02) {
				total_inodes++;
			}
			if ((*p) & 0x04) {
				total_inodes++;
			}
			if ((*p) & 0x02) {
				total_inodes++;
			}
			if ((*p) & 0x08) {
				total_inodes++;
			}
			if ((*p) & 0x10) {
				total_inodes++;
			}
			if ((*p) & 0x20) {
				total_inodes++;
			}
			if ((*p) & 0x40) {
				total_inodes++;
			}
			if ((*p) & 0x80) {
				total_inodes++;
			}
			p++;
		}
	}
	str = '\0';
	char buf[100];
	sprintf(buf, "total_blocks:\t%hu\nfree_blocks:\t%hu\nused_blocks:\t%hu\ntotal_inodes:\t%d\n",
		total_blocks, free_blocks, used_blocks, total_inodes);
	strcpy(str, buf);
}

void ps_info(char * str) {
	struct task_struct ** p;
	str = "pid\tstate\tfather\tcount\tstart_time\n";
	for(p = &LAST_TASK ; p > &FIRST_TASK ; --p) {
		if (*p) {
			char buf[50];
			sprintf(buf, "%ld\t%ld\t%ld\t%ld\t%ld\n", (*p)->pid, (*p)->state, (*p)->father, (*p)->counter, (*p)->start_time);
			strcat(str, buf);
		}
	}		
}

int proc_read(int dev, struct file * filp, char * buf, int count)
{
	if (count == 0) {
		return 0;
	}
	char * str = malloc(4096);
	if (dev == 0) {
		ps_info(str);
	}
	if (dev == 1) {
		hd_infos(str);
	}
	int len = strlen(str);
	int f_pos = filp -> f_pos;
	if (f_pos + count + 1 > len) {
		count = len - f_pos - 1;
	}
	if (count == 0) {
		free(str);
		return 0;
	}
	int i;
	for (i = 0; i < count; i++) {
		put_fs_byte(str[f_pos + i], buf);
	}
	filp -> f_pos = f_pos + count;
	free(str);
	return count;

}
