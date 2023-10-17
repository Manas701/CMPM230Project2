#include <iostream>
#include <SFML/Graphics.hpp>
#include "button.h"

const float windowLength = 800.0f;
const float indentPercent = 0.05;
const float indentWidth = windowLength * indentPercent; // 40
const float lineWidth = 1.0f;
const float gridLength = windowLength - (2*indentWidth); // 720
const int gridSize = 32; // 40 to 760
const float squareLength = gridLength / gridSize; // 22.5

const float paletteLength = 200;
const float paletteHeight = 400;
const int paletteAcross = 2;
const int paletteDown = 4;
const float tileWidth = paletteLength/paletteAcross;
const float tileHeight = paletteHeight/paletteDown;

const float buttonWindowLength = 200;
const float buttonWindowHeight = 800;
const float buttonLength = 100;
const float buttonIndent = 10;

const std::string pathToAssets = "../../src/assets/";
int tileMap[gridSize][gridSize] = {};
std::map<int, sf::Texture> paletteMap;
int currentTile = 1;
int selectedTile = 1;

int brushSize = 0;

sf::Texture grassTile, flowerTile, waterTile, fishTile, roadTile, carTile, grassHumanTile, waterHumanTile;

Button drawButton, eraseButton, eyedropButton, sizeUpButton, sizeDownButton, saveButton, loadButton;
enum currentTool {Draw, Erase, Eyedropper};
currentTool tool = Draw;
Button selectedButton;

sf::RenderWindow window(sf::VideoMode(windowLength, windowLength), "Tilemap :)");
sf::RenderWindow palette(sf::VideoMode(paletteLength, paletteHeight), "Palette :p");
sf::RenderWindow buttons(sf::VideoMode(buttonWindowLength, buttonWindowHeight), "Buttons :D");

void Button::initButton(std::string name, float length)
{
    this->name = name;
    this->length = length;
    this->rect = sf::RectangleShape(sf::Vector2f(length, length));
    this->rect.setFillColor(sf::Color(172,202,232));
    this->rect.move((buttons.getSize().x - length)/2, buttonIndent+(buttonIndent+length)*this->order);
    this->xBounds = sf::Vector2f(this->rect.getPosition().x, this->rect.getPosition().x+length);
    this->yBounds = sf::Vector2f(this->rect.getPosition().y, this->rect.getPosition().y+length);
}

void initButtons()
{
    drawButton.order = 0;
    drawButton.initButton("DRAW", buttonLength);
    selectedButton = drawButton;

    eraseButton.order = 1;
    eraseButton.initButton("ERASE", buttonLength);

    eyedropButton.order = 2;
    eyedropButton.initButton("EYEDROPPER", buttonLength);

    sizeUpButton.order = 3;
    sizeUpButton.initButton("BRUSH UP", buttonLength);

    sizeDownButton.order = 4;
    sizeDownButton.initButton("BRUSH DOWN", buttonLength);

    saveButton.order = 5;
    saveButton.initButton("SAVE", buttonLength);

    loadButton.order = 6;
    loadButton.initButton("LOAD", buttonLength);
}

bool Button::checkWithinBounds()
{
    if (sf::Mouse::getPosition(buttons).x >= this->xBounds.x && sf::Mouse::getPosition(buttons).x <= this->xBounds.y && sf::Mouse::getPosition(buttons).y >= this->yBounds.x && sf::Mouse::getPosition(buttons).y <= this->yBounds.y)
    {
        return true;
    }
    return false;
}

bool Button::checkIfClicked()
{
    if (this->checkWithinBounds() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        return true;
    }
    return false;
}

void drawButtonAction()
{
    if (tool != Draw)
    {
        currentTile = selectedTile;
        tool = Draw;
        selectedButton = drawButton;
    }
}

void eraseButtonAction()
{
    if (tool != Erase)
    {
        selectedTile = currentTile;
        tool = Erase;
        selectedButton = eraseButton;
    }
}

void eyedropButtonAction()
{
    if (tool != Eyedropper)
    {
        tool = Eyedropper;
        selectedButton = eyedropButton;
    }
}

void sizeUpAction()
{
    if (brushSize < 8)
    {
        brushSize += 1;
    }
}

void sizeDownAction()
{
    if (brushSize > 0)
    {
        brushSize -= 1;
    }
}

void checkButtons()
{
    if (drawButton.checkIfClicked())
    {
        drawButtonAction();
    }
    if (eraseButton.checkIfClicked())
    {
        eraseButtonAction();
    }
    if (eyedropButton.checkIfClicked())
    {
        eyedropButtonAction();
    }
    if (sizeUpButton.checkIfClicked())
    {
        sizeUpAction();
    }
    if (sizeDownButton.checkIfClicked())
    {
        sizeDownAction();
    }
}

void drawButtons()
{
    buttons.draw(drawButton.rect);
    buttons.draw(eraseButton.rect);
    buttons.draw(eyedropButton.rect);
    buttons.draw(sizeUpButton.rect);
    buttons.draw(sizeDownButton.rect);
    buttons.draw(saveButton.rect);
    buttons.draw(loadButton.rect);

    sf::RectangleShape heldIndicator(sf::Vector2f(buttonLength, buttonLength));
    heldIndicator.setPosition(selectedButton.rect.getPosition());
    heldIndicator.setFillColor(sf::Color(0, 0, 0, 100)); // transparent gray
    buttons.draw(heldIndicator);
}

void loadTexture(sf::Texture *tileName, std::string tilePath, int key)
{
    if (!tileName->loadFromFile(pathToAssets+tilePath))
    {
        std::cout << "Asset: " << pathToAssets << tilePath << "failed to load\n";
    }
    paletteMap[key] = (*tileName);
}

void loadPalette()
{
    loadTexture(&grassTile, "GrassTile.png", 1);
    loadTexture(&flowerTile, "FlowerTile.png", 2);
    loadTexture(&waterTile, "WaterTile.png", 3);
    loadTexture(&fishTile, "FishTile.png", 4);
    loadTexture(&roadTile, "RoadTile.png", 5);
    loadTexture(&carTile, "CarTile.png", 6);
    loadTexture(&grassHumanTile, "GrassHumanTile.png", 7);
    loadTexture(&waterHumanTile, "WaterHumanTile.png", 8);
}

void drawPaletteIndicator()
{
    float tileMouseX = sf::Mouse::getPosition(palette).x;
    float tileMouseY = sf::Mouse::getPosition(palette).y;

    // draw an indicator for which tile you have currently selected
    sf::RectangleShape heldIndicator(sf::Vector2f(tileWidth, tileHeight));
    int tempTile = currentTile;
    if (currentTile == 0)
    {
        tempTile = 0;
        currentTile = selectedTile;
    }
    int heldX = (currentTile % paletteAcross) - 1;
    int heldY = currentTile / paletteAcross;
    if (heldX == -1)
    {
        heldX = paletteAcross - 1;
        heldY -= 1;
    }
    heldIndicator.setPosition(heldX*tileWidth,heldY*tileHeight);
    heldIndicator.setFillColor(sf::Color(0, 0, 0, 100)); // transparent gray
    palette.draw(heldIndicator);

    // get the tile coordinates of the mouse on the palette
    if (tileMouseX<0 || tileMouseX>paletteLength || tileMouseY<0 || tileMouseY>paletteHeight)
    {
        currentTile = tempTile;
        return;
    }
    tileMouseX = int(tileMouseX / tileWidth);
    tileMouseY = int(tileMouseY / tileHeight);

    // draw an indicator for the tile currently being hovered over
    sf::RectangleShape hoverIndicator(sf::Vector2f(tileWidth, tileHeight));
    hoverIndicator.setPosition(tileMouseX*tileWidth, tileMouseY*tileHeight);
    hoverIndicator.setFillColor(sf::Color(255, 255, 255, 100)); // transparent gray
    palette.draw(hoverIndicator);


    currentTile = tempTile;

    // change to the selected tile
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        currentTile = tileMouseY * paletteAcross + (tileMouseX + 1);
        selectedTile = currentTile;
    }
}

void drawPalette()
{
    for (int i=0; i<paletteDown;i++)
    {
        for (int j=0; j<paletteAcross;j++)
        {
            sf::Sprite tile;
            tile.setTexture(paletteMap[(i*paletteAcross)+j+1]);
            tile.setScale(sf::Vector2f(tileWidth/tile.getLocalBounds().width, tileHeight/tile.getLocalBounds().height));
            tile.setPosition((j*tileWidth), i*tileWidth);
            palette.draw(tile);
        }
    }

    drawPaletteIndicator();
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
        sf::RectangleShape hoverIndicator(sf::Vector2f(squareLength*(2*brushSize+1)-lineWidth, squareLength*(2*brushSize+1)-lineWidth));
        hoverIndicator.setPosition(indentWidth+((std::get<0>(tilePosition)-brushSize)*squareLength), indentWidth+lineWidth+((std::get<1>(tilePosition)-brushSize)*squareLength));
        hoverIndicator.setFillColor(sf::Color(255, 255, 255, 100)); // transparent gray
        window.draw(hoverIndicator);
    }
}

void alterGrid()
{
    std::tuple<int, int> tilePosition = getTilePosition();
    int tilePosX = std::get<0>(tilePosition);
    int tilePosY = std::get<1>(tilePosition);
    if (tilePosX >= 0)
    {
        if (tool == Draw || tool == Erase)
        {
            for (int i=(-brushSize);i<=brushSize;i++)
            {
                for (int j=(-brushSize);j<=brushSize;j++)
                {
                    if (tilePosX+i >= 0 && tilePosX+i <= (gridSize-1) && tilePosY+j >= 0 && tilePosY+j <= (gridSize-1))
                    {
                        tileMap[tilePosX+i][tilePosY+j] = currentTile;
                    }
                }
            }
        }
        else
        {
            if (tileMap[tilePosX][tilePosY] > 0)
            {
                currentTile = tileMap[tilePosX][tilePosY];
                selectedTile = currentTile;
            }
        }
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
                tile.setTexture(paletteMap[tileMap[i][j]]);
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
    //window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width*0.1, 0));
    palette.setPosition(sf::Vector2i(window.getPosition().x+windowLength+indentWidth, windowLength*0.25));
    buttons.setPosition(sf::Vector2i(window.getPosition().x-buttonWindowLength-indentWidth, windowLength*0));

    initButtons();

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
        buttons.clear();

        if (tool == Erase)
        {
            currentTile = 0;
        }

        //drawing stuff here
        drawButtons();
        checkButtons();


        //draw tiles on palette
        drawPalette();

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
        buttons.display();
    }
}