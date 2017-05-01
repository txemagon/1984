
#include <stdlib.h>
#include <stdio.h>
#include <png.h>

struct Timage{
int width;
int height;
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers;
};

struct Timage image;

void read_png_file(char *filename) {
  FILE *fp = fopen(filename, "rb");

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png) abort();

  png_infop info = png_create_info_struct(png);
  if(!info) abort();

  if(setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  png_read_info(png, info);

  image.width      = png_get_image_width(png, info);
  image.height     = png_get_image_height(png, info);
  image.color_type = png_get_color_type(png, info);
  image.bit_depth  = png_get_bit_depth(png, info);

  // Read any color_type into 8bit depth, RGBA format.
  // See http://www.libpng.org/pub/png/libpng-manual.txt

  if(image.bit_depth == 16)
    png_set_strip_16(png);

  if(image.color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
  if(image.color_type == PNG_COLOR_TYPE_GRAY && image.bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if(png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // These color_type don't have an alpha channel then fill it with 0xff.
  if(image.color_type == PNG_COLOR_TYPE_RGB ||
     image.color_type == PNG_COLOR_TYPE_GRAY ||
     image.color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  if(image.color_type == PNG_COLOR_TYPE_GRAY ||
     image.color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * image.height);
  for(int y = 0; y < image.height; y++) {
    row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
  }

  png_read_image(png, row_pointers);

  fclose(fp);

  png_destroy_read_struct(&png, &info, NULL);
    png=NULL;
    info=NULL;
}

void write_png_file(char *filename) {
  int y;

  FILE *fp = fopen(filename, "wb");
  if(!fp) abort();

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) abort();

  png_infop info = png_create_info_struct(png);
  if (!info) abort();

  if (setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  // Output is 8bit depth, RGBA format.
  png_set_IHDR(
    png,
    info,
    image.width, image.height,
    8,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );
  png_write_info(png, info);

  // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
  // Use png_set_filler().
  //png_set_filler(png, 0, PNG_FILLER_AFTER);

  png_write_image(png, row_pointers);
  png_write_end(png, NULL);

  for(int y = 0; y < image.height; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);

  fclose(fp);

  if (png && info)
        png_destroy_write_struct(&png, &info);
}

void process_png_file() {
  for(int y = 0; y < image.height; y++) {
    png_bytep row = row_pointers[y];
    for(int x = 0; x < image.width; x++) {
      png_bytep px = &(row[x * 4]);
      // Do something awesome for each pixel here...
      //printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
    }
  }
}

int main(int argc, char *argv[]) {
  if(argc != 3) abort();

  read_png_file(argv[1]);
  process_png_file();
  write_png_file(argv[2]);

  return 0;
}