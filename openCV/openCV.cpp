#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

void drawWorld(const vector<vector<int>>& matrix)
{
    vector<string> imagePaths = {
        "C:\\images\\tile_ground.png",
        "C:\\images\\tile_water.png",
        "C:\\images\\tile_forest.png",
        "C:\\images\\tile_field.png",
        "C:\\images\\tile_iron_mine.png",
        "C:\\images\\tile_blocks_mine.png"
    };

    vector<Mat> tiles;
    for (const auto& path : imagePaths)
    {
        Mat img = imread(path, IMREAD_COLOR);
        if (img.empty())
        {
            cerr << "Could not read the image: " << path << endl;
            return;
        }
        tiles.push_back(img);
    }

    int rows = matrix.size();
    int cols = matrix[0].size();

    Mat world(rows * 101 + 1, cols * 101 + 1, CV_8UC3, Scalar(0, 0, 0));

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            int tileIndex = matrix[i][j] - 1;
            if (tileIndex >= 0 && tileIndex < 6)
            {
                Mat tile = tiles[tileIndex];

                int roi_x = j * 101 + 1;
                int roi_y = i * 101 + 1;
                Rect roi(roi_x, roi_y, 100, 100);

                if (roi.x >= 0 && roi.y >= 0 &&
                    roi.x + roi.width <= world.cols && roi.y + roi.height <= world.rows)
                {
                    Mat targetROI = world(roi);
                    resize(tile, tile, Size(100, 100));
                    tile.copyTo(targetROI);
                }
                else
                {
                    cerr << "Error: ROI coordinates out of bounds." << endl;
                    return;
                }
            }
        }
    }

    namedWindow("World", WINDOW_NORMAL);
    setWindowProperty("World", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN); // הצגת החלון במסך מלא

    imshow("World", world);
    waitKey(0);
}

int main()
{
    vector<vector<int>> matrix = {
        {1, 2, 3, 4, 5, 6},
        {6, 5, 4, 3, 2, 1},
        {1, 2, 3, 4, 5, 6},
        {6, 5, 4, 3, 2, 1},
        {1, 2, 3, 4, 5, 6},
        {6, 5, 4, 3, 2, 1}
    };

    drawWorld(matrix);

    return 0;
}
