#pragma once
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class DrawWorld
{
    vector<vector<int>> Board;
    vector<string> imagePaths;
    vector<Mat> tiles;

public:
    DrawWorld() {};
    void setBoard(const vector<vector<int>>& worldMap)
    {
        // הכפלת המטריצה פי 5 לכל כיוון
        int rows = worldMap.size();
        int cols = worldMap[0].size();
        Board.resize(rows * 5, vector<int>(cols * 5));

        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                int value = worldMap[i][j];
                for (int k = 0; k < 5; ++k)
                {
                    for (int l = 0; l < 5; ++l)
                    {
                        Board[i * 5 + k][j * 5 + l] = value;
                    }
                }
            }
        }

        // נתיבי התמונות
        imagePaths = {
            "C:\\images\\tile_ground.png",
            "C:\\images\\tile_water.png",
            "C:\\images\\tile_forest.png",
            "C:\\images\\tile_field.png",
            "C:\\images\\tile_iron_mine.png",
            "C:\\images\\tile_blocks_mine.png"
        };

        // טעינת התמונות המתאימות
        for (const auto& path : imagePaths)
        {
            Mat img = imread(path, IMREAD_COLOR);
            if (img.empty())
            {
                cerr << "Could not read the image: " << path << endl;
                continue;
            }
            tiles.push_back(img);
        }
    }

    void drawWorld()
    {
        int rows = Board.size();
        int cols = Board[0].size();

        // יצירת התמונה הסופית בגודל המתאים
        Mat world(rows * 101 + 1, cols * 101 + 1, CV_8UC3, Scalar(0, 0, 0));

        // ציור המטריצה עם התמונות
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                int tileIndex = Board[i][j] - 1;
                if (tileIndex >= 0 && tileIndex < tiles.size())
                {
                    Mat tile = tiles[tileIndex];

                    // בדיקת תקינות ה־ROI
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

        // יצירת חלון להצגת העולם
        namedWindow("World", WINDOW_NORMAL);
        setWindowProperty("World", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN); // הצגת החלון במסך מלא

        // הצגת התמונה הסופית
        imshow("World", world);
        waitKey(0);
    }

    void AddBuilding(int x, int y) { Board[x][y] = 7; drawWorld(); } // הוסף ערך מתאים לתמונה של בניין
    void AddCity(int x, int y) { Board[x][y] = 8; drawWorld(); } // הוסף ערך מתאים לתמונה של עיר
    void AddRoad(int x, int y) { Board[x][y] = 9; drawWorld(); } // הוסף ערך מתאים לתמונה של כביש
    void AddPeople(int x, int y) { Board[x][y] = 10; drawWorld(); } // הוסף ערך מתאים לתמונה של אנשים
};
