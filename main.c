/**
* @file main.c
* main file of the program in C
* @author Alexandre Djossou
* @date 13 november 2022
* @version 1.0
* @copyright GNU Public License
*/
/* Import libraries */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "gd.h" //gd library
#include "gdfontl.h" //gdfontl library

#define PI 3.141592654

/************************Start Functions Declaration**************************/
//Function createdFilledArc Declaration
void createdFilledArc(gdImagePtr, int, int, int, int, int , int , int , char*);

//Function help
void help();

//Function splitString Declaration
void splitString(char*, const char*, char***, int* );
/**************************End Functions Declaration*************************/

/*****************************Start Function main****************************/
/**
 * @brief main function
 * @param argc: number of argument
 * @param argv: array of arguments in string format
 * @return 0
 */
int main(int argc, char *argv[]) {
  /* Declare the image */
  gdImagePtr im;
  /* Declare output files */
  FILE *pngout;
  /* Declare color indexes */
  int backgroundColor;
  int edgesColor;
  int startAngle;
  int endAngle;
  int differenceAngle;
  int needHelp = 0;
  int percentage;
  int R_slice;
  int G_slice;
  int B_slice;
  int sliceColor;
  int opt;
  int R_bg = 0;
  int G_bg = 0;
  int B_bg = 0;
  int R_edges = 255;
  int G_edges = 255;
  int B_edges = 255;
  int width = 600;
  int height = 600;
  int sizeChart = 400;
  int num_slices = 0;
  int num_rgb_slices = 0;
  int num_labels=0;
  int num_percentages = 0;
  int percentages_arr_int[num_percentages+50];
  int slices_color_arr_int[num_slices*3+50];

  char *label = NULL;
  char *filename;
  char *background_color_str;
  char *edges_color_str;
  char *chart_size_str;
  char *image_size_str;
  char *labels_str;
  char *percentages_str;
  char *slices_color_str = NULL;
  char **labels_arr_str;
  char labels_array[100][100];
  char percentages_array[100][100];

//*****Start Options Extraction*****//
  while((opt = getopt(argc, argv, ":b:e:f:i:l:p:s:c:h")) != -1)
  {
      switch(opt)
      {
          case 'b':
              background_color_str = optarg;
              break;
          case 'e':
              edges_color_str = optarg;
              break;
          case 'f':
              filename = optarg;
              break;
          case 'c':
              chart_size_str = optarg;
              break;
          case 'h':
              needHelp = 1;
              break;
          case 'i':
              image_size_str = optarg;
              break;
          case 'l':
              labels_str = optarg;
              break;
          case 'p':
              percentages_str = optarg;
              break;
          case 's':
              slices_color_str = optarg;
              break;
          case ':':
              printf("option needs a value\n");
              break;
          case '?':
              printf("unknown option: %c\n", optopt);
              break;
      }
  }

  // optind is for the extra arguments
  // which are not parsed
  for(; optind < argc; optind++){
      printf("extra arguments: %s\n", argv[optind]);
  }

  //******Start Help******//
    if(needHelp){
        help();
    }
  //******End Help******//

  if((percentages_str != NULL) && (filename != NULL)){
  printf("\n**********************************************\n");
  printf("\n***********Summary of the pie chart***********\n");

  printf("\n- Output filename: %s.png\n", filename);

  //background color processing
  if (background_color_str != NULL){
      int count=0;
      char **bg_color_arr_str;
      char arr_temp[strlen(background_color_str)];
      strcpy(arr_temp, background_color_str);
      splitString(arr_temp, "-", &bg_color_arr_str, &count);
      R_bg = atoi(bg_color_arr_str[0]);
      G_bg = atoi(bg_color_arr_str[1]);
      B_bg = atoi(bg_color_arr_str[2]);

      printf("\n- Background color:\n");

      printf("R: %d\n", R_bg);
      printf("G: %d\n", G_bg);
      printf("B: %d\n", B_bg);

  }

  //chart size processing
  if (chart_size_str != NULL){
      sizeChart = atoi(chart_size_str);

      printf("\n- Chart size:\n");

      printf("chart width: %d\n", sizeChart);
      printf("chart height: %d\n", sizeChart);
  }

  //edges color processing
  if (edges_color_str != NULL){
      int count=0;
      char **edges_color_arr_str;
      char arr_temp[strlen(edges_color_str)];
      strcpy(arr_temp, edges_color_str);
      splitString(arr_temp, "-", &edges_color_arr_str, &count);

      R_edges = atoi(edges_color_arr_str[0]);
      G_edges = atoi(edges_color_arr_str[1]);
      B_edges = atoi(edges_color_arr_str[2]);

      printf("\n- Edges color:\n");

      printf("R: %d\n", R_edges);
      printf("G: %d\n", G_edges);
      printf("B: %d\n", B_edges);
  }

  //image size processing
  if (image_size_str != NULL){
      int count=0;
      char **image_arr_str;
      char arr_temp[strlen(image_size_str)];
      strcpy(arr_temp, image_size_str);
      splitString(arr_temp, "-", &image_arr_str, &count);

      width = atoi(image_arr_str[0]);
      height = atoi(image_arr_str[1]);

      printf("\n- Image size:\n");

      printf("image width: %d\n", width);
      printf("image height: %d\n", height);
  }

  //labels processing
  if (labels_str != NULL){
      char arr_str_1[strlen(labels_str)];

      strcpy(arr_str_1, labels_str);
      splitString(arr_str_1, "-", &labels_arr_str, &num_labels);

      printf("\n- Pie chart labels:\n");

      for (int i = 0; i<num_labels; i++){
          strcpy(labels_array[i], labels_arr_str[i]);
          printf("label %d: %s\n", i, labels_array[i]);
      }
  }

  //percentages processing
  if (percentages_str != NULL){
      char **percentages_arr_str;
      char arr_str_2[strlen(percentages_str)];
      strcpy(arr_str_2, percentages_str);
      splitString(arr_str_2, "-", &percentages_arr_str, &num_percentages);

      for (int i = 0; i<num_percentages; i++){
          percentages_arr_int[i] = atoi(percentages_arr_str[i]);
      }

      printf("\n- Pie chart slices' percentages:\n");

      for (int i = 0; i<num_percentages; i++){
          strcpy(percentages_array[i], percentages_arr_str[i]);
          printf("percentage slice %d: %s\n", i, percentages_array[i]);
      }
  }

  //slices color processing
  if (slices_color_str != NULL){
      char arr_temp_1[strlen(slices_color_str)+1];
      char **arr_temp_2;
      strcpy(arr_temp_1, slices_color_str);
      splitString(arr_temp_1, "/", &arr_temp_2, &num_slices);

      printf("\n- Pie chart slices' colors:\n");

      for (int i = 0; i<num_slices; i++){
          printf("color slice %d: %s\n", i, arr_temp_2[i]);

          char arr_temp_3[strlen(arr_temp_2[i])];
          char **arr_temp_4;
          strcpy(arr_temp_3, arr_temp_2[i]);
          splitString(arr_temp_3, "-", &arr_temp_4, &num_rgb_slices);

          slices_color_arr_int[3*i] = atoi(arr_temp_4[0]);
          slices_color_arr_int[3*i+1] = atoi(arr_temp_4[1]);
          slices_color_arr_int[3*i+2] = atoi(arr_temp_4[2]);
      }
  }
//******End Options Extraction******//

//******Start Creation of the Pie Chart******//

      /* Allocate the image: 64 pixels across by 64 pixels tall */
      im = gdImageCreate(width, height);

      /* Since this is the first color in a new image, it will
        be the background color. */
      backgroundColor = gdImageColorAllocate(im, R_bg, G_bg, B_bg);

      /* Allocate the color pie chart edges color. */
      edgesColor = gdImageColorAllocate(im, R_edges, G_edges, B_edges);

      /* Initialize the start angle to 270 */
      startAngle = 270;

      for(int i=0; i<num_percentages; i++){
          if(slices_color_str!=NULL){
              R_slice = slices_color_arr_int[3*i];
              G_slice = slices_color_arr_int[3*i+1];
              B_slice = slices_color_arr_int[3*i+2];
          } else {
              R_slice = rand() % 255;
              G_slice = rand() % 255;
              B_slice = rand() % 255;
          }

          sliceColor = gdImageColorAllocate(im, R_slice, G_slice, B_slice);

          percentage = atoi(percentages_array[i]);
          label = labels_array[i];

          differenceAngle = (int)(360*percentage/100.0);
          endAngle = startAngle + differenceAngle;

          createdFilledArc(im, width/2, height/2, sizeChart,
                          startAngle, endAngle, sliceColor,
                          edgesColor, label);

          startAngle = endAngle;
      }

      /* Open a file for writing. "wb" means "write binary", important
        under MSDOS, harmless under Unix. */
        strcat(filename, ".png");
        pngout = fopen(filename, "wb");

      /* Output the image to the disk file in PNG format. */
      gdImagePng(im, pngout);

      /* Close the files. */
      fclose(pngout);

      /* Destroy the image in memory. */
      gdImageDestroy(im);
  }
//******End Creation of the Pie Chart******//
}
/******************************End Function main*****************************/

/******************************Start Function createdFilledArc*****************************/
/**
 * @brief createdFilledArc
 * creates a filled arc
 * @param image: gdImagePtr pointer
 * @param center_x: abscissa of the image's center
 * @param center_y: ordinate of the image's center
 * @param size: 2*radius of the arc
 * @param startAngle: start angle of the arc
 * @param endAngle: end angle of the arc
 * @param color_filling: filling color of the arc
 * @param color_border: border color of the arc
 * @param label: label of the arc
 */
void createdFilledArc(gdImagePtr image, int center_x, int center_y,
                     int size, int startAngle, int endAngle,
                     int color_filling, int color_border,
                     char* label){

    /* Draw a red filled arc */
    gdImageFilledArc(image, (int)center_x, (int)center_y,
                     size, size, startAngle, endAngle,
                     color_filling, gdPie);
    gdImageArc (image, (int)center_x, (int)center_y,
                size, size, startAngle, endAngle,
                color_border);

    double arg_debut_rad, arg_fin_rad, arg_label_rad;
    int x_debut, y_debut, x_fin, y_fin, x_0_line_label,
        y_0_line_label, x_1_line_label, y_1_line_label,
        x_label, y_label;

    arg_debut_rad = (startAngle * PI) / 180;
    arg_fin_rad = (endAngle * PI) / 180;

    x_debut = (int)center_x + (size/2) * cos(arg_debut_rad);
    y_debut = (int)center_y + (size/2) * sin(arg_debut_rad);

    x_fin = (int)center_x + (size/2) * cos(arg_fin_rad);
    y_fin = (int)center_y + (size/2) * sin(arg_fin_rad);

    gdImageLine(image, (int)center_x, (int)center_y, x_debut, y_debut, color_border);
    gdImageLine(image, (int)center_x, (int)center_y, x_fin, y_fin, color_border);

    if(label != NULL){
        /* Draw label line */
        arg_label_rad = (startAngle+((endAngle-startAngle)/2)) * PI / 180;
        x_0_line_label = (int)center_x + (size/2) * cos(arg_label_rad);
        y_0_line_label = (int)center_y + (size/2) * sin(arg_label_rad);
        x_1_line_label = x_0_line_label + (5) * cos(arg_label_rad);
        y_1_line_label = y_0_line_label + (5) * sin(arg_label_rad);

        gdImageLine(image, x_0_line_label, y_0_line_label, x_1_line_label, y_1_line_label, color_border);

        /* Print label */
        //if (startAngle+((endAngle-startAngle)/2)>150 && startAngle+((endAngle-startAngle)/2)<200){
        if(startAngle+((endAngle-startAngle)/2)>450 && startAngle+((endAngle-startAngle)/2)<550){
        //if (startAngle>450 && startAngle<500){
            x_label = x_1_line_label + (10*strlen(label) + 5) * cos(arg_label_rad);
            y_label = y_1_line_label + (10) * sin(arg_label_rad);
            gdImageString(image, gdFontLarge, x_label, y_label, (unsigned char*)label, color_border);
        } else if(startAngle+((endAngle-startAngle)/2)>550 && startAngle+((endAngle-startAngle)/2)<600){
            x_label = x_1_line_label + (15*strlen(label) + 5) * cos(arg_label_rad);
            y_label = y_1_line_label-15;
            gdImageString(image, gdFontLarge, x_label, y_label, (unsigned char*)label, color_border);
        } else if(startAngle+((endAngle-startAngle)/2)>600){
            x_label = x_1_line_label + (15*strlen(label) + 5) * cos(arg_label_rad);
            y_label = y_1_line_label-20;
            gdImageString(image, gdFontLarge, x_label, y_label, (unsigned char*)label, color_border);
        } else {
            x_label = x_1_line_label + (5) * cos(arg_label_rad);
            y_label = y_1_line_label + 5 * sin(arg_label_rad);
            gdImageString(image, gdFontLarge, x_label, y_label, (unsigned char*)label, color_border);
        }
    }
}
/******************************End Function createPieChart*****************************/

/******************************Start Function help*****************************/
/**
 * @brief help
 * displays a help menu
 */
void help(){
printf("****************************************************************  help  ****************************************************************\n");
printf("This program, written in C language, plots a pie chart and saves \
it to a .png file. The program is based on the gd library \nand has many \
options. The percentages and the filename are \
mandatory parameters. \nThe program is used via the command line \
interface. \n\n\
Options: \n\
-b: background color in RGB of the image. For example, in order to \
have a black background, type \"-b 0-0-0\".\n\
-c: size in pixels of the chart. For example, to have an pie chart \
of 200 pixels across by 200 pixels tall, type \"-i 200.\"\n\
-e: color in RGB for the edges and the labels. For example, in order to \
have a white color, type \"-e 255-255-255\".\n\
-f: output filename.\n\
-h: help to explain how to use the program.\n\
-i: size in pixels of the image. For example, to have an output image \
of 500 pixels across by 500 pixels tall, type \"-i 500-500.\"\n\
-l: labels for the chart. For a pie chart of three slices, the labels \
are given as follows \"-l africa-asia-europe\".\n\
-p: percentages of the slices. For a pie chart of three slices, the \
percentages are given as follows \"-p 20-30-50\".\n\
-s: color in RGB of the slices. For a pie chart of three slices, the \
colors are given as follows \"-s 255-255-205/150-10-150/175-7-72.\"\n");
}
/******************************End Function help*****************************/

/******************************Start Function splitString*****************************/
/**
 * @brief splitString
 * splits a string in tokens
 * @param str: string to split
 * @param delim: delimiter in char format
 * @param array: array of strings to store the tokens
 * @param count: number of tokens in the array
 */
void splitString(char *str, const char *delim, char ***array, int *count ) {
  int i=0;
  char *token;
  char **res = (char **) malloc(0 * sizeof(char *));

  /* get the first token */
   token = strtok(str, delim);
   while( token != NULL )
   {
        res = (char **) realloc(res, (i + 1) * sizeof(char *));
        res[i] = token;
        i++;
      token = strtok(NULL, delim);
   }
   *array = res;
   *count = i;
}
/******************************End Function splitString*****************************/



