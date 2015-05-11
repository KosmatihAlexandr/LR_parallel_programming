#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

IplImage *image=0, *dst=0;
double r,g,b; int num;

void* fthread(void *arg) 
{
	for(int i = int(double((*(int*)arg)*image->height)/double(num));
	i < ((*(int*)arg)+1)*image->height/double(num); i++)
		for(int j = 0; j < image->width; j++) {
			CvScalar c = cvGet2D(image,i,j);
			c.val[0]=c.val[0]*b+c.val[1]*g+c.val[2]*r;
			cvSet2D(dst, i, j, c);
		}
}

int main(int argc, char* argv[])
{
	pthread_t *thread;
	int *arg;
        // имя картинки
	if(argc < 2) {
		printf("не указан входной файл");
		return -1;
	}
        char *filename = argv[1];
	if(argc > 2) num = atoi(argv[2]);
	else num = 4;
	if(argc == 6) {
		r = atof(argv[3]);
		g = atof(argv[4]);
		b = atof(argv[5]);
		if(r+g+b > 1) {
			printf("недопустимое соотношение цветов");
			r = 0.33; g = 0.33; b = 0.33;
		}
	}
	else {
		r = 0.33; g = 0.33; b = 0.33;
	}
        // получаем картинку
        image = cvLoadImage(filename, 1);
        printf("image: %s\n", filename);
        assert( image != 0 );
	// создание чёрно-белого изображения
	dst = cvCreateImage(cvGetSize (image), image->depth, 1);

	thread = new pthread_t[num];
	arg = new int[num];
	for (int i = 0; i < num; i++) {
		arg[i] = i;
		pthread_create(&thread[i], 0, fthread, &arg[i]);
	}
	for (int i = 0; i < num; i++) {
		pthread_join(thread[i], 0);
	}
	cvSaveImage("res.jpg", dst);
/*        // вывод изображения на экран
        cvNamedWindow( "image" );
        cvShowImage( "image", image );
	cvNamedWindow( "result" );
        cvShowImage( "result", dst );
        // ожидание нажатия клавиши
        cvWaitKey(0);
*/        // освобождаем ресурсы
        cvReleaseImage(& image);
        cvReleaseImage(&dst);
	free(thread);
	free(arg);
        // удаляем окна
        cvDestroyAllWindows();
        return 0;
}
