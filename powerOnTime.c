//http://pointer-overloading.blogspot.in/2013/09/linux-creating-entry-in-proc-file.html*Hello!*/

#include <linux/module.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/time.h>

#define DRIVER_AUTHOR "Gabriel Reus Rodriguez"
#define DRIVER_DESC "Un módulo que calcula el tiempo que lleva el ordenador encendido"
#define DRIVER_LICENSE "GPL"
#define DRIVER_VERSION "0.1"

static struct timespec initialTime;
static struct formatedTime{
	unsigned short seconds;
	unsigned short minutes;
	unsigned short hours;
	unsigned int days;
};

static struct formatedTime formatTimeStamp(struct timespec *time)
{
	struct formatedTime humanTime;

	humanTime.seconds = time->tv_sec % 60;
	humanTime.minutes = time->tv_sec / 60;
	humanTime.hours   = ( time->tv_sec / 3600 ) % 24;
	humanTime.days	  =  ( time->tv_sec / 3600 ) / 24;

	return humanTime;
}

static void diff_between_timestamps(struct timespec init, struct timespec *time)
{
	time->tv_sec =  time->tv_sec - init.tv_sec;
	time->tv_nsec = time->tv_nsec - init.tv_nsec;
}

static void getTimeStamp(struct timespec *time)
{
	getnstimeofday(time);

/*void getnstimeofday(struct timespec *ts) struct timespec ts_start,ts_end,test_of_time;


printk("TIME: %.2lu:%.2lu:%.2lu:%.6lu \r\n",
                   (curr_tm.tv_sec / 3600) % (24),
                   (curr_tm.tv_sec / 60) % (60),
                   curr_tm.tv_sec % 60,
                   curr_tm.tv_nsec / 1000);

       #include <sys/time.h>

       int gettimeofday(struct timeval *tv, struct timezone *tz);	

	struct timeval time;
	unsigned long local_time;

	do_gettimeofday(&time);
	local_time = (u32)(time.tv_sec - (sys_tz.tz_minuteswest * 60));
	rtc_time_to_tm(local_time, &tm);

	printk(" @ (%04d-%02d-%02d %02d:%02d:%02d)\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);spec

	time_t t = time(0);
	time_t SecondsOfDay = t % (24*60*60);
	time_t HourGMT = SecondsOfDay / (60*60);
*/
}

static int powerOnTime_proc_show(struct seq_file *m, void *v)
{
	struct timespec *time;
	struct formatedTime humanTime;

	getTimeStamp(time);
	diff_between_timestamps(initialTime,time);
	humanTime = formatTimeStamp(time);	
  	seq_printf(m, "powerOn time: %d days %d hours %d minutes %d seconds\n",humanTime.days,humanTime.hours,humanTime.minutes,humanTime.seconds);
  	return 0;
}

static int powerOnTime_proc_open(struct inode *inode, struct  file *file)
{

  return single_open(file, powerOnTime_proc_show, NULL);
}

static const struct file_operations powerOnTime_proc_fops = {
  .owner = THIS_MODULE,
  .open = powerOnTime_proc_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};

static int __init init(void)
{
	pr_info("Initiating powerOnTime module\n");
	proc_create("powerOnTime", 0, NULL, &powerOnTime_proc_fops);
	getTimeStamp(&initialTime);
	pr_info("powerOnTime module initiated\n");
	return 0;
}

static void __exit cleanup(void)
{
	remove_proc_entry("powerOnTime", NULL);
	pr_info("powerOnTime module unloaded\n");
}

module_init(init);
module_exit(cleanup);

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERSION);
