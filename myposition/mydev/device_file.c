//#include "device_file.h"
#include <linux/fs.h> 	     /* file stuff */
#include <linux/kernel.h>    /* printk() */
#include <linux/errno.h>     /* error codes */
#include <linux/module.h>  /* THIS_MODULE */
#include <linux/cdev.h>      /* char device stuff */
#include <asm/uaccess.h>  /* copy_to_user() */
#include <linux/jiffies.h>  /*where jiffies define*/

static char    g_s_position_string[30];
static ssize_t g_s_position_size = sizeof(g_s_position_string);
static unsigned long seed = 0;            
static int x = 10000,y = 10000,z = 10000;
/*===============================================================================================*/
static void generate_position(void)  //generate random x,y,z with 'linear congruential method'
{
  if (seed == 0)
  {
    seed = jiffies;   //inital_seed = get_time_now
  }
  seed = ( seed * 9301 + 49297 ) % 233280;    //best prarmeters
  x = (seed>116640)?(x+(seed-116640)/200):(x-(116640-seed)/200);  //not support float
  seed = ( seed * 9301 + 49297 ) % 233280;    
  y = (seed>116640)?(y+(seed-116640)/200):(y-(116640-seed)/200);  //not support float
  seed = ( seed * 9301 + 49297 ) % 233280;    
  z = (seed>116640)?(z+(seed-116640)/200):(z-(116640-seed)/200);  //not support float

  sprintf(g_s_position_string,"%5d,%5d,%5d",x,y,z);
}

/*===============================================================================================*/
static ssize_t device_file_read(struct file *file_ptr, char __user *user_buffer, size_t count, loff_t *possition){
  printk(KERN_NOTICE "Simple-driver: Device file is read at offset = %i, read bytes count = %u",
    (int)*possition, (unsigned int)count);
  if(*possition >= g_s_position_size)
    return 0;
  if(*possition + count > g_s_position_size)
    count = g_s_position_size - *possition;
  generate_position();
  if(raw_copy_to_user(user_buffer, g_s_position_string + *possition, count) != 0)
    return -EFAULT;
  *possition += count;
  return count;
}

/*===============================================================================================*/
static struct file_operations simple_driver_fops = {
   .owner   = THIS_MODULE,
   .read    = device_file_read,
};

static int device_file_major_number = 201;

static const char device_name[] = "Simple-driver";

/*===============================================================================================*/
int register_device(void){
  int result = 0;
  printk( KERN_NOTICE "Simple-driver: register_device() is called." );
  result = register_chrdev( device_file_major_number, device_name, &simple_driver_fops );
  if(result < 0){
    printk( KERN_WARNING "Simple-driver:  can\'t register character device with errorcode = %i", result );
    return result;
  }
  printk(KERN_NOTICE "Simple-driver: registered character device with major number = %i and minor numbers 0...255",
    device_file_major_number);
  return 0;
}

/*-----------------------------------------------------------------------------------------------*/
void unregister_device(void){
   printk(KERN_NOTICE "Simple-driver: unregister_device() is called");
   unregister_chrdev(device_file_major_number, device_name);
}
