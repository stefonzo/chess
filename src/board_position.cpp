#include "board.h"
#include <ostream>
#include <string>



board_position_t::board_position_t(size_t index) {
	this->row = index / 8;
	this->col = index % 8;
}


size_t board_position_t::index() {
	return (8*row) + col;
}


std::string board_position_t::colName() {
	switch (col) {
	case 0:
		return "A";
	case 1:
		return "B";
	case 2:
		return "C";
	case 3:
		return "D";
	case 4:
		return "E";
	case 5:
		return "F";
	case 6:
		return "G";
	case 7:
		return "H";
	default:
		return std::to_string(col);
	}
}


std::ostream& operator<< (std::ostream &outs, board_position_t p) {
	return outs << p.colName() << std::to_string(p.row);
}
