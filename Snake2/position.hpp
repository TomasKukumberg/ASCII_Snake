#pragma once

class Position
{
private:
    int x;
    int y;
public:
    Position();
    Position(int x, int y);
    int getX();
    void setX(int x);
    int getY();
    void setY(int y);
    bool operator == (const Position& other) const;
};

