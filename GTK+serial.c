#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <gtk/gtk.h> 
#include <wiringPi.h>
#include <wiringSerial.h>

int count = 0; 
int tom = 0;
int fd ;
char deviceActive [12];
double baudrate;
//GtkWidget *timer;

void end_program (GtkWidget *wid, gpointer ptr) 
{ 
  gtk_main_quit (); 
  //gtk_widget_destroy();
} 

void count_button (GtkWidget *wid, gpointer ptr) 
{ 
  char buffer[30]; 
  count++; 
  if (tom==0)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
  {
    tom=1;
    /*digitalWrite (LED, LOW) ;
    digitalWrite (LED1, LOW) ;
    digitalWrite (LED2, LOW) ;
    digitalWrite (LED3, LOW) ;*/
    sprintf (buffer, "lampu padam ");
    //printf("lampu padam"); //ini akan tampil saat event telah selesai
    g_print("lampu padam \n");	// ini akan tampil seketika walaupun event belum selesai
  }else if (tom==1)
  {
    tom=0;
    /*digitalWrite (LED, HIGH) ;
    digitalWrite (LED1, HIGH) ;
    digitalWrite (LED2, HIGH) ;
    digitalWrite (LED3, HIGH) ;*/
    sprintf (buffer, "lampu nyalah ");
    g_print ("lampu nyalah \n");
    //sprintf (buffer, "Button pressed %d times", count); 
  }
  gtk_label_set_text (GTK_LABEL (ptr), buffer); 
}


void serialJalan (GtkWidget *wid, gpointer ptr)
{
  int tes33=0;
  char deviceActive2 [30];//="/dev/ttyUSB0";
  
  g_print("menggunakan usbttl actve di: %s ",deviceActive);  //ingat ada /n di akhir deviceactiv ini akibat /n di selected saat pemilihan device
  //strncpy(deviceActive2, deviceActive,12);	//strncpy(c, "string", 6);
  g_print("%d",strlen(deviceActive));//sizeof(deviceActive));
  g_print("nilai baudrate: %0.0f\n", baudrate);

  //getchar();
  strcpy(deviceActive2, deviceActive);
  int size = strlen(deviceActive2); //Total size of selected karena ada /n yang harus dihapus
  deviceActive2[size-1] = '\0';

  if ((fd = serialOpen (deviceActive2, baudrate)) < 0)  //("/dev/ttyUSB0", baudrate)) < 0)  //("/dev/ttyUSB0", 115200)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    //return 1 ;
  }

/**  
  if (wiringPiSetup () == -1)
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    //return 1 ;
  }
**/
// Loop, getting and printing characters
/**
 while (serialDataAvail (fd))
    {
      //printf (" -> %3d", serialGetchar (fd)) ;
      putchar (serialGetchar (fd)) ;
      fflush (stdout) ;
    }
/**
char tks[100];
static GString *time = NULL;
 
     if(time == NULL)
     {
         time = g_string_sized_new(10);
     }
**/

  for (;;)	//blok ini meneyebabkan hang
  //while (tes33<100)//(fd!=10)//serialDataAvail (fd))
  {
    char t= serialGetchar(fd);
    if (t=='\n'){
      //serialFlush(fd);
      //serialClose(fd);
      //g_print("%s--",deviceActive2);
      fflush (stdout) ;
      serialClose(fd);
      break;
    }
    
    printf("%c",t); //serialGetchar(fd)); //ini menghasilkan nilai nilai integer????
    //tes33++;

    //g_print ("%d", serialGetchar (fd)); 
    //sprintf (tks,"%s",serialGetchar (serialOpen ("/dev/ttyUSB0", 9600)) ) ; 
    //g_print("%s",tks);
    //putchar (serialGetchar (fd)) ;
    //g_string_printf(time,"%d",serialGetchar (fd));<
    //g_print("%d",tes33);
    fflush (stdout) ;
    //serialClose(fd);
    
    //if (serialGetchar (fd)==107){//(fd==10){
    //  serialClose(fd);
    //  break;
      
    //}
   
    tes33++;
    
  }
  
  //serialClose(fd);

/**
  while (serialDataAvail (fd))
  {
      printf (" -> %3d", serialGetchar (fd)) ;
      fflush (stdout) ;
  }
**/
  
}


void combo_changed (GtkWidget *wid, gpointer ptr) 
{ 
  char teks [30];
  int sel = gtk_combo_box_get_active (GTK_COMBO_BOX (wid)); 
  char *selected = gtk_combo_box_text_get_active_text ( GTK_COMBO_BOX_TEXT (wid)); 
  
  //ini penting
 // int size = strlen(selected); //Total size of selected karena ada /n yang harus dihapus
 // selected[size-1] = '\0';
  
  
  //strcpy(teks,selected);
  sprintf(deviceActive,"/dev/%s",selected);
  //g_string_printf(deviceActive,"/dev/%s",teks);
  //strcpy(deviceActive,selected);
  //g_print("%s ---",deviceActive);	// perhatikan disi selalu ganti baris
  g_print(" %d ", strlen(selected)); //seharusnya 7 untuk ttyUSB0 tapi kebaca 8
  g_print("%s ---",selected);	
  
  g_print ("The value of the combo is %d %s---\n", sel, deviceActive); 
}

void combo_changed_baud (GtkWidget *wid, gpointer ptr) 
{ 
  int sel = gtk_combo_box_get_active (GTK_COMBO_BOX (wid)); 
  char *selected = gtk_combo_box_text_get_active_text ( GTK_COMBO_BOX_TEXT (wid)); 
  //strcpy(deviceActive, selected);
  baudrate = atof(selected);
  g_print("%0.0f",baudrate);
  g_print ("The value of the baudrate is %d %s---\n", sel, selected); 
}


int main( int argc, char **argv )
{
  
  FILE *fp;
  char path[1035];
  int jumlahDev,i,j;
  char buff[100][100];
  
  /* Open the command for reading. */ 
  fp = popen("ls /sys/class/tty/ |tail ","r"); //ls -al /dev/tty* | grep USB","r");  // ///bin/ls /etc/", "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }
  
  /* Read the output a line at a time - output it. */ 
  while (fgets(path, sizeof(path), fp) != NULL) {
    //printf(" %d %s",i, path);
    //strcpy(buff[i],path);
    strncpy(buff[i],path,sizeof(path));
    //memcpy(buff[i],path,sizeof(path));
    i++;
    jumlahDev=i;
  }
  j=i;
  
  for(i=0;i<j;i++){
     printf(" %d %s",i, buff[i]); //nah disini kelihatan ada /n(newline) padahal gakada /n
   }
  //printf(" 5. %s", buff[5]);
  
  /* close */ 
  pclose(fp); 
  
  gtk_init (&argc, &argv); 
  
  //window
  GtkWidget *win = gtk_window_new (GTK_WINDOW_TOPLEVEL); 
  gtk_window_set_title (GTK_WINDOW (win), "Serial");
  gtk_window_set_default_size (GTK_WINDOW (win), 250, 400);
  gtk_container_set_border_width (GTK_CONTAINER (win), 10);
  gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);
  g_signal_connect (win, "delete_event", G_CALLBACK (end_program), NULL); 
  
  //label dan button 
  GtkWidget *btn = gtk_button_new_with_label ("Close"); 
  g_signal_connect (btn, "clicked", G_CALLBACK (end_program), NULL); 
  GtkWidget *btn1 = gtk_button_new_with_label ("connect"); 
  g_signal_connect (btn1, "clicked", G_CALLBACK (serialJalan), NULL); 
  
  GtkWidget *btn2 = gtk_button_new_with_label ("saklar"); 
  GtkWidget *lbl = gtk_label_new ("Lampu"); 
  g_signal_connect (btn2, "clicked", G_CALLBACK (count_button), lbl); 
  
  //combo select /dev/tty
  GtkWidget *comb = gtk_combo_box_text_new (); 
  for(i=0;i<j;i++){
    char buff2[30];
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (comb), buff[i]);//sprintf(buff2,"%s",buff[i]));//"Option 1"); 
    gtk_combo_box_set_active (GTK_COMBO_BOX (comb), 0);
   }
   g_signal_connect (comb, "changed", G_CALLBACK (combo_changed), deviceActive);
  
  //combo select baudrate
  GtkWidget *combBaud = gtk_combo_box_text_new (); 
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combBaud), "4800"); 
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combBaud), "9600"); 
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combBaud), "115200"); 
  gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combBaud), "220000"); 
  gtk_combo_box_set_active (GTK_COMBO_BOX (combBaud), 0);
  g_signal_connect (combBaud, "changed", G_CALLBACK (combo_changed_baud), NULL);
  
  
/**  timer = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(timer),"---");
 
**/
 
  
  //box orientasi vertical	
  GtkWidget *box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5); //GTK_ORIENTATION_HORIZONTAL, 5); 
  gtk_box_pack_start (GTK_BOX (box), btn2, TRUE, FALSE, 0); //sakalar
  gtk_box_pack_start (GTK_BOX (box), lbl, TRUE, TRUE, 0); 
  gtk_box_pack_start (GTK_BOX (box), comb, TRUE, TRUE, 0); 
  gtk_box_pack_start (GTK_BOX (box), combBaud, TRUE, TRUE, 0); 
  gtk_box_pack_start (GTK_BOX (box), btn1, TRUE, TRUE, 0);  //connect
  gtk_box_pack_start (GTK_BOX (box), btn, TRUE, TRUE, 0);  //close
//  gtk_container_add(GTK_CONTAINER (box), timer);
	
  gtk_container_add (GTK_CONTAINER (win), box); 
  gtk_widget_show_all (win); 
  gtk_main (); 
 
  return 0;
}
