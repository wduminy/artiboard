#pragma once
#include <iostream>
#include <set>

namespace arti {
	typedef short ordinal_t; 
	/*
	* A square identifies a position on the gameboard
	* A square is the ordered pair (file,rank) where the left-bottom
	* is (1,1) and top-right is (8,8).
	* A square is either light or dark. left-bottom is light.
	* Squares are also indexed in to ways: 0 to 63, and by color index
	* 0 to 31.
	*/
	class Square
	{
	public:
		// check if f and r is from 1 to 8
		static bool in_bounds(ordinal_t f, ordinal_t r)	{ return (f > 0) & (f < 9)& (r > 0) & (r < 9); }
		enum color_t {Light, Dark};
		// index for a valid square is 0 to 63
		ordinal_t index(void) const;
		ordinal_t rank() const {return _rank;}
		ordinal_t file() const {return _file;}
		bool is_valid() const {return in_bounds(_file,_rank);}
		ordinal_t color_index(const bool flip = false) const;
		color_t color() const;

		// normal constructor
		Square(const ordinal_t file=1, const ordinal_t rankValue=1);
		// contruct a flip of another square
		Square(const Square& source, bool flip=false);
		// constucts an offset square
		Square(const Square& source, 
			const ordinal_t offset_file, 
			const ordinal_t offset_rank);
		Square(const ordinal_t dark_color_index);
		bool operator < (const Square& other) const { return index() < other.index();}
		bool operator == (const Square& other) const { return index() == other.index(); }
	private:
		/* updates */
		// change to the mirror of the square
		void flip()
		{
			_file = 9 - _file;
			_rank = 9 - _rank;
		}
		/* queries */
		// return true if params equals file and rank
		bool is(ordinal_t f, ordinal_t r) const
		{
			return (_file == f && _rank == r);
		}

	private:
		ordinal_t _file, _rank;
	};
	std::ostream & operator  <<(std::ostream &, const Square&) ;

		// union = U
	// intersect = I
	class Region:public std::set<Square>
	{
	public:
		// inserts the valid neighbours of the middle square into this
		void insert_diag_neighbours(const Square& middle);
		// inserts the valid second neighbours of the middle square into this
		void insert_diag_second_neighbours(const Square& middle);
		// adds s to this if s is valid
		void operator += (const Square &s)
		{ if (s.is_valid()) insert(s); }
		// this = this U (color squares of rank(r))
		void insert_rank(const ordinal_t r, const Square::color_t color);
	private:

		bool contains(const Square & s) const
		{
			return find(s) != end();
		}
		/* intersect_count returns the number of elements in this that * that 
		are also in s. */
		int intersect_count(const Region & s) const;
		// this = this U s
		void insert_set(const Region & s);

		// t = this I row(r): rows starts from 1
		void intersect_rank(Region & t, const ordinal_t r) const;

		// t = this I s
		void set_intersect(Region & t, const Region & s) const;

		// t = this U s
		void set_union(Region & t, const Region & s) const;

		/* flip Changes the state to the other players view */
		void flip();

		

		// removes the squares of the given color
		void remove_by_color(const Square::color_t color);

	};
	std::ostream & operator  <<(std::ostream &, const Region &);
	std::istream & operator  >>(std::istream &, Region &);

}