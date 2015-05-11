#include <cv.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/param.h>
#include <mpi.h>

int main(int argc, char** argv) 
{
	IplImage *image, *dst;
	int i, j, comp, task;
	int height, width;
	double r,g,b;
	int* heights = 0;
	int root = 0, num, rank, is_root, recvcount;
	int *sendcounts = 0, *displs = 0;
	unsigned char *image_data_buf;
	if(argc == 5) {
		r = atof(argv[2]);
		g = atof(argv[3]);
		b = atof(argv[4]);
		if(r+g+b > 1) {
			printf("недопустимое соотношение цветов");
			r = 0.33; g = 0.33; b = 0.33;
		}
	}
	else {
		r = 0.33; g = 0.33; b = 0.33;
	}
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num);
	is_root = rank == root;
	if (is_root) {
		sendcounts = calloc(num, sizeof(int));
		displs = calloc(num, sizeof(int));
		heights = calloc(num, sizeof(int));
		// получаем картинку
		image = cvLoadImage(argv[1], 1);
		printf("image: %s\n", argv[1]);
		assert( image != 0 );
		// создание чёрно-белого изображения
		dst = cvCreateImage(cvGetSize (image), image->depth, 1);
		width = image->width;
		task = image->height / num;
		for (i = 0; i < num - 1; i++) {
			heights[i] = task;
		}
		heights[num - 1] = image->height - task *
					      (num - 1);
		for (i = 0; i < num; i++) {
			displs[i] = i * task * width * 3;
			sendcounts[i] = heights[i] * width * 3;
		}
	}
	MPI_Scatter(sendcounts, 1, MPI_INT, &recvcount, 1, MPI_INT, root,
		    MPI_COMM_WORLD);
	MPI_Scatter(heights, 1, MPI_INT, &height, 1, MPI_INT,
		    root, MPI_COMM_WORLD);
	MPI_Bcast(&width, 1, MPI_INT, root, MPI_COMM_WORLD);
	image_data_buf = calloc(recvcount, sizeof(unsigned char));
	MPI_Scatterv(image->imageData, sendcounts, displs, MPI_UNSIGNED_CHAR,
		     image_data_buf, recvcount, MPI_UNSIGNED_CHAR, root,
		     MPI_COMM_WORLD);
	unsigned char (*image_data)[width][3] = image_data_buf;
	unsigned char (*dst_image_data)[width] = 
			calloc(recvcount/3, sizeof(unsigned char));;
	for(i = 0; i < height; i++)
		for(j = 0; j < width; j++){
			dst_image_data[i][j] = image_data[i][j][0]*b+
			image_data[i][j][1]*g+image_data[i][j][2]*r;;
			}
	if (is_root)
		for (i = 0; i < num; i++) {
			displs[i] = i * task * width;
			sendcounts[i] = heights[i] * width;
		}
	MPI_Gatherv(dst_image_data, recvcount/3, MPI_UNSIGNED_CHAR,
		    dst->imageData, sendcounts, displs, MPI_UNSIGNED_CHAR,
		    root, MPI_COMM_WORLD);
	if (is_root) {
		cvSaveImage("res.jpg", dst);
		cvReleaseImage(&image);
	}
	free(image_data_buf);
	free(heights);
	free(displs);
	free(sendcounts);
	MPI_Finalize();
}
