/*
 * powerOnTime	A module that says how much time has been turned on the machine.
 *
 * Version:	@(#)powerOnTime.c	1.0.0	22.01.2016
 *
 * Authors:	Gabriel Reus Rodriguez, <gabrielin@gmail.com>
 *
 * Fixes:
 */

#include <linux/module.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/time.h>

#define DRIVER_AUTHOR "Gabriel Reus Rodriguez"
#define DRIVER_DESC "Un módulo que calcula el tiempo que lleva el ordenador encendido"
#define DRIVER_LICENSE "GPL"
#define DRIVER_VERSION "0.1"

/*
 * Declaración de estructuras
 *
 */

struct formatedTime {
	unsigned short seconds;
	unsigned short minutes;
	unsigned short hours;
	unsigned int days;
};

/*
 * Declaración de variables
 */

static struct timespec initialTime;

/*
 *Función que da formato al tiempo
 */
static struct formatedTime formatTimeStamp(struct timespec time)
{
	struct formatedTime humanTime;

	humanTime.seconds = time.tv_sec % 60;
	humanTime.minutes = (time.tv_sec / 60) % 60;
	humanTime.hours   = (time.tv_sec / 3600) % 24;
	humanTime.days	  = (time.tv_sec / 3600) / 24;

	return humanTime;
}

/*
 * Calcula la diferencia entre dos timestamps y la deja en el segundo timestamp
 */
static void diff_between_timestamps(struct timespec init, struct timespec *time)
{
	time->tv_sec =  time->tv_sec - init.tv_sec;
	time->tv_nsec = time->tv_nsec - init.tv_nsec;
}

/*
 * Obtiene el timeStamp
 */
static void getTimeStamp(struct timespec *time)
{
	getnstimeofday(time);
}

/*
 * Función que implementa la lógica del callback.
 */
static int powerOnTime_proc_show(struct seq_file *m, void *v)
{
	struct timespec time;
	struct formatedTime humanTime;

	getTimeStamp(&time);
	diff_between_timestamps(initialTime, &time);
	humanTime = formatTimeStamp(time);
	seq_printf(m, "powerOn time:\t%3d days, %2d hours, %2d minutes, %2d seconds\n",
		humanTime.days,
		humanTime.hours,
		humanTime.minutes,
		humanTime.seconds);
	return 0;
}

/*
 * Callback que se ejecutará al abrir el fichero.
 */
static int powerOnTime_proc_open(struct inode *inode, struct  file *file)
{
	return single_open(file, powerOnTime_proc_show, NULL);
}

/*
 * Definimos los callbacks del fichero
 */
static const struct file_operations powerOnTime_proc_fops = {
	.owner = THIS_MODULE,
	.open = powerOnTime_proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release
};

static int __init init(void)
{
	pr_info("Initiating powerOnTime module\n");
	/*
	 * Método para crear el fichero en una carpeta
	 * struct proc_dir_entry *parent = proc_mkdir("gabriel",NULL);
	 * proc_create("powerOnTime", 0, parent, &powerOnTime_proc_fops);
	*/
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
