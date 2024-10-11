struct Position {
    short row;
    short column;

    Position() = default;

    Position(short r, short c)
        : row(r)
        , column(c) {}

    bool operator==(Position& other) { return row == other.row && column == other.column; }
};
