#include <iostream>
#include <SFML/Graphics.hpp>

const float windowLength = 800.0f;
const float indentPercent = 0.05;
const float indentWidth = windowLength * indentPercent; // 40
const float lineWidth = 1.0f;
const float gridLength = windowLength - (2*indentWidth); // 720
const int gridSize = 32; // 40 to 760
const float squareLength = gridLength / gridSize; // 22.5
const float paletteLength = 200;
const float paletteHeight = 400;
const std::string pathToAssets = "../../src/assets/";
int tileMap[gridSize][gridSize] = {};
std::map<int, sf::Texture> paletteMap;

sf::RenderWindow window(sf::VideoMode(windowLength, windowLength), "Tilemap :)");
sf::RenderWindow palette(sf::VideoMode(paletteLength, paletteHeight), "Palette :I");

void loadPalette()
{
    sf::Texture grassTile;
    if (!grassTile.loadFromFile(pathToAssets+"GrassTile.png"))
    {
        std::cout << "darn" << std::endl;
    }
    paletteMap[1] = grassTile;
}

void drawGrid()
{
    sf::RectangleShape line(sf::Vector2f(gridLength, lineWidth));

    for (int i=0; i <= gridSize; i++)
    {
        // draw a horizontal line
        line.setRotation(0);
        line.setPosition(0, gridLength/gridSize * i);
        line.move(indentWidth, indentWidth);
        window.draw(line);

        // draw a vertical line
        line.setRotation(90);
        line.setPosition(gridLength/gridSize * i, 0);
        line.move(indentWidth, indentWidth);
        window.draw(line);
    }
}

std::tuple<int, int> getTilePosition()
{
    float gridPosX = sf::Mouse::getPosition(window).x - indentWidth;
    float gridPosY = sf::Mouse::getPosition(window).y - indentWidth;
    // if mouse is not within grid, don't give a tile position
    if (gridPosX < 0 || gridPosX > gridLength || gridPosY < 0 || gridPosY > gridLength)
    {
        return std::make_tuple(-1,-1);
    }
    return std::make_tuple(gridPosX / squareLength, gridPosY / squareLength);
}

void drawIndicator()
{
    std::tuple<int, int> tilePosition = getTilePosition();
    if (std::get<0>(tilePosition) >= 0)
    {
        sf::RectangleShape hoverIndicator(sf::Vector2f(squareLength-lineWidth, squareLength-lineWidth));
        hoverIndicator.setPosition(indentWidth+(std::get<0>(tilePosition)*squareLength), indentWidth+lineWidth+(std::get<1>(tilePosition)*squareLength));
        hoverIndicator.setFillColor(sf::Color(255, 255, 255, 100)); // transparent gray
        window.draw(hoverIndicator);
    }
}

void alterGrid()
{
    std::tuple<int, int> tilePosition = getTilePosition();
    if (std::get<0>(tilePosition) >= 0)
    {
        tileMap[std::get<0>(tilePosition)][std::get<1>(tilePosition)] = 1;
    }
}

void drawTiles()
{
    for (int i=0;i<gridSize;i++)
    {
        for (int j=0;j<gridSize;j++)
        {
            if (tileMap[i][j] > 0)
            {
                sf::Sprite tile;
                tile.setTexture(paletteMap[tileMap[i][j]], true);
                tile.setScale(sf::Vector2f(squareLength/tile.getLocalBounds().width, squareLength/tile.getLocalBounds().height));
                tile.setPosition(indentWidth+(i*squareLength), indentWidth+lineWidth+(j*squareLength));
                window.draw(tile);
            }
            else {
                sf::RectangleShape emptyTile(sf::Vector2f(squareLength-lineWidth, squareLength-lineWidth));
                emptyTile.setPosition(indentWidth+(i*squareLength), indentWidth+lineWidth+(j*squareLength));
                emptyTile.setFillColor(sf::Color(0, 0, 0)); // black
                window.draw(emptyTile);
            }
        }
    }
}

int main()
{
    window.setPosition(sf::Vector2i(200, 0));
    palette.setPosition(sf::Vector2i(window.getPosition().x+windowLength+indentWidth, windowLength*0.25));

    loadPalette();

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                for (int i=0;i<gridSize;i++)
                {
                    for (int j=0;j<gridSize;j++)
                    {
                        std::cout << tileMap[j][i] << "  ";
                    }
                    std::cout << "\n";
                }
                window.close();
            }
        }

        window.clear();
        palette.clear();

        //drawing stuff here

        //draw grid
        drawGrid();

        //alter arrays to represent drawn tiles
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            alterGrid();
        }

        //draw painted tiles
        drawTiles();

        //draw transparent gray box over selected tile
        drawIndicator();


        window.display();
        palette.display();
    }
}