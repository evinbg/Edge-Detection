#include <iostream>
#include <cmath>
#define HEIGHT 750
#define WIDTH 750
#define THRESHOLD 24 // edit this to change the edge detection sensitivity (lower = more edges will be detected)

void detectEdges(unsigned char** img, unsigned char** edgeImg);
void gradient(unsigned char** img, unsigned char** gradientImgX, unsigned char** gradientImgY);
unsigned char** sobel3x3(unsigned char img[HEIGHT][WIDTH], int sobel[3][3]);
unsigned char** sobel5x5(unsigned char img[HEIGHT][WIDTH], int sobel[5][5]);

int main() {
	unsigned char img[HEIGHT][WIDTH];

	std::string fileName = "image.raw";
	const char* fn = (fileName).c_str(); // to convert string to const char*
	FILE* photo = fopen(fn, "rb");

	if (!photo) {
		std::cout << "Input file \"" + fileName + "\"not found" << std::endl;
		return 1;
	}

	fread(img, 1, HEIGHT * WIDTH, photo);

	// sobel 3x3 operator matrices
	int sobel_3x3_X[3][3] = {
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}
	};
	int sobel_3x3_Y[3][3] = {
		{-1, -2, -1},
		{0, 0, 0},
		{1, 2, 1}
	};

	// sobel 5x5 operator matrices
	int sobel_5x5_X[5][5] = {
		{-1, -2, 0, 2, 1},
		{-2, -4, 0, 4, 2},
		{-4, -8, 0, 8, 4},
		{-2, -4, 0, 4, 2},
		{-1, -2, 0, 2, 1}
	};
	int sobel_5x5_Y[5][5] = {
		{-1, -2, -4, -2, -1},
		{-2, -4, -8, -4, -2},
		{0, 0, 0, 0, 0},
		{2, 4, 8, 4, 2},
		{1, 2, 4, 2, 1}
	};

	// creating 2D array on heap for edge and gradient images
	unsigned char** edgeImg_3x3;
	unsigned char** edgeImg_5x5;
	unsigned char** gradientImg_3x3;
	unsigned char** gradientImg_5x5;
	edgeImg_3x3 = new unsigned char* [HEIGHT];
	edgeImg_5x5 = new unsigned char* [HEIGHT];
	gradientImg_3x3 = new unsigned char* [HEIGHT];
	gradientImg_5x5 = new unsigned char* [HEIGHT];

	// setting the images to all black initially
	for (int i = 0; i < HEIGHT; i++) {
		edgeImg_3x3[i] = new unsigned char[WIDTH];
		edgeImg_5x5[i] = new unsigned char[WIDTH];
		gradientImg_3x3[i] = new unsigned char[WIDTH];
		gradientImg_5x5[i] = new unsigned char[WIDTH];
		for (int j = 0; j < WIDTH; j++) {
			edgeImg_3x3[i][j] = 0;
			edgeImg_5x5[i][j] = 0;
			gradientImg_3x3[i][j] = 0;
			gradientImg_5x5[i][j] = 0;
		}
	}

	// getting the 2D arrays of the sobel filter applied to the image
	unsigned char** sobelImg_3x3_X = sobel3x3(img, sobel_3x3_X);
	unsigned char** sobelImg_3x3_Y = sobel3x3(img, sobel_3x3_Y);
	unsigned char** sobelImg_5x5_X = sobel5x5(img, sobel_5x5_X);
	unsigned char** sobelImg_5x5_Y = sobel5x5(img, sobel_5x5_Y);

	// gradient images
	gradient(gradientImg_3x3, sobelImg_3x3_X, sobelImg_3x3_Y);
	gradient(gradientImg_5x5, sobelImg_5x5_X, sobelImg_5x5_Y);

	// detecting the edges of the sobel filtered images
	detectEdges(gradientImg_3x3, edgeImg_3x3);
	detectEdges(gradientImg_5x5, edgeImg_5x5);

	// converting 2D heap array back to 2D stack array
	unsigned char outEdge3x3[HEIGHT][WIDTH];
	unsigned char outEdge5x5[HEIGHT][WIDTH];
	unsigned char outGradient3x3[HEIGHT][WIDTH];
	unsigned char outGradient5x5[HEIGHT][WIDTH];

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			outEdge3x3[i][j] = edgeImg_3x3[i][j];
			outEdge5x5[i][j] = edgeImg_5x5[i][j];
			outGradient3x3[i][j] = gradientImg_3x3[i][j];
			outGradient5x5[i][j] = gradientImg_5x5[i][j];
		}
	}

	// writing 2D arrays to files
	FILE* outputEdge3x3 = fopen("edge3x3.raw", "wb");
	fwrite(outEdge3x3, 1, HEIGHT * WIDTH, outputEdge3x3);
	fclose(outputEdge3x3);

	FILE* outputEdge5x5 = fopen("edge5x5.raw", "wb");
	fwrite(outEdge5x5, 1, HEIGHT * WIDTH, outputEdge5x5);
	fclose(outputEdge5x5);

	FILE* outputGradient3x3 = fopen("gradient3x3.raw", "wb");
	fwrite(outGradient3x3, 1, HEIGHT * WIDTH, outputGradient3x3);
	fclose(outputGradient3x3);

	FILE* outputGradient5x5 = fopen("gradient5x5.raw", "wb");
	fwrite(outGradient5x5, 1, HEIGHT * WIDTH, outputGradient5x5);
	fclose(outputGradient5x5);

	return 0;
};

void detectEdges(unsigned char** img, unsigned char** edgeImg) {
	// for loops starting at 1 and ending at -1 to prevent sobel calculation outside of image
	for (int i = 1; i < HEIGHT - 1; i++) {
		for (int j = 1; j < WIDTH - 1; j++) {

			int mid = img[i][j];

			if (mid > THRESHOLD) { // if current point would have a change in color big enough to be considered an edge

				int left = img[i][j - 1];
				int right = img[i][j + 1];
				int top = img[i + 1][j];
				int bottom = img[i - 1][j];
				int horizontal_comparison = (mid - left) + (mid - right);
				int veritcal_comparison = (mid - top) + (mid - bottom);

				if (horizontal_comparison > veritcal_comparison) { // if the horizontal points have a bigger difference (they are more of an edge)
					if (mid > left && mid > right) { // if it is a peak
						(edgeImg)[i][j] = 200;
					}
				}
				else if (horizontal_comparison < veritcal_comparison) { // if the vertical points have a bigger difference (they are more of an edge)
					if (mid > top && mid > bottom) { // if it is a peak
						(edgeImg)[i][j] = 200;
					}
				}
			}
		}
	}
}

void gradient(unsigned char** img, unsigned char** gradientImgX, unsigned char** gradientImgY) {
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			long long int expX = (abs(128 - (gradientImgX)[i][j]) * abs(128 - (gradientImgX)[i][j]));
			long long int expY = (abs(128 - (gradientImgY)[i][j]) * abs(128 - (gradientImgY)[i][j]));
			long long int exp = expX + expY;
			long int sqroot = sqrt(exp);
			(img)[i][j] = sqroot;
		}
	}
}

unsigned char** sobel3x3(unsigned char img[HEIGHT][WIDTH], int sobel[3][3]) {
	unsigned char** outimg;
	outimg = new unsigned char* [HEIGHT];

	// for loops starting at 1 and ending at -1 to prevent sobel calculation outside of image
	for (int i = 0; i < HEIGHT; i++) {
		outimg[i] = new unsigned char[WIDTH];
		if (i > 0 && i < HEIGHT - 1) {
			for (int j = 1; j < WIDTH - 1; j++) {
				int sobel_op =
					img[i - 1][j - 1] * sobel[0][0] +
					img[i - 1][j] * sobel[0][1] +
					img[i - 1][j + 1] * sobel[0][2] +
					img[i][j - 1] * sobel[1][0] +
					img[i][j] * sobel[1][1] +
					img[i][j + 1] * sobel[1][2] +
					img[i + 1][j - 1] * sobel[2][0] +
					img[i + 1][j] * sobel[2][1] +
					img[i + 1][j + 1] * sobel[2][2];

				int sobel_op_normalized = 128 + (sobel_op / 6);
				if (sobel_op_normalized > 255) {
					sobel_op_normalized = 255;
				}
				else if (sobel_op_normalized < 0) {
					sobel_op_normalized = 0;
				}
				(outimg)[i][j] = sobel_op_normalized;
			}
		}
	}

	return outimg;
}

unsigned char** sobel5x5(unsigned char img[HEIGHT][WIDTH], int sobel[5][5]) {
	unsigned char** outimg;
	outimg = new unsigned char* [HEIGHT];

	// for loops starting at 2 and ending at -2 to prevent sobel calculation outside of image
	for (int i = 0; i < HEIGHT; i++) {
		outimg[i] = new unsigned char[WIDTH];
		if (i > 1 && i < HEIGHT - 2) {
			for (int j = 2; j < WIDTH - 2; j++) {
				int sobel_op =
					img[i - 2][j - 2] * sobel[0][0] +
					img[i - 2][j - 1] * sobel[0][1] +
					img[i - 2][j] * sobel[0][2] +
					img[i - 2][j + 1] * sobel[0][3] +
					img[i - 2][j + 2] * sobel[0][4] +

					img[i - 1][j - 2] * sobel[1][0] +
					img[i - 1][j - 1] * sobel[1][1] +
					img[i - 1][j] * sobel[1][2] +
					img[i - 1][j + 1] * sobel[1][3] +
					img[i - 1][j + 2] * sobel[1][4] +

					img[i][j - 2] * sobel[2][0] +
					img[i][j - 1] * sobel[2][1] +
					img[i][j] * sobel[2][2] +
					img[i][j + 1] * sobel[2][3] +
					img[i][j + 2] * sobel[2][4] +

					img[i + 1][j - 2] * sobel[3][0] +
					img[i + 1][j - 1] * sobel[3][1] +
					img[i + 1][j] * sobel[3][2] +
					img[i + 1][j + 1] * sobel[3][3] +
					img[i + 1][j + 2] * sobel[3][4] +

					img[i + 2][j - 2] * sobel[4][0] +
					img[i + 2][j - 1] * sobel[4][1] +
					img[i + 2][j] * sobel[4][2] +
					img[i + 2][j + 1] * sobel[4][3] +
					img[i + 2][j + 2] * sobel[4][4];

				int sobel_op_normalized = 128 + (sobel_op / 40);
				if (sobel_op_normalized > 255) {
					sobel_op_normalized = 255;
				}
				else if (sobel_op_normalized < 0) {
					sobel_op_normalized = 0;
				}

				(outimg)[i][j] = sobel_op_normalized;
			}
		}
	}

	return outimg;
}
