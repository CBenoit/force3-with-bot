#ifndef GAMESQUARE_HPP
#define GAMESQUARE_HPP

#include <QWidget>
#include <QColor>


#define AS_COLORS(x,y) x,
#define AS_PLAYERS(x,y) y,
#define COLORS_AND_PLAYERS(ENTRY) \
	ENTRY(Qt::GlobalColor::red, red) \
	ENTRY(Qt::GlobalColor::blue, blue) \
	ENTRY(Qt::GlobalColor::black, empty_square) \
	ENTRY(Qt::GlobalColor::white, available)

namespace square {

	enum class type: unsigned char {
		COLORS_AND_PLAYERS(AS_PLAYERS)
		type_number
	};

	static const std::array<QColor,static_cast<int>(type::type_number)> COLORS = {{ COLORS_AND_PLAYERS(AS_COLORS) }};
}

class Gamesquare : public QWidget
{
	Q_OBJECT

public:

	explicit Gamesquare(QWidget *parent = nullptr);

	void type(square::type new_type);

	square::type type() const;

	void swap(Gamesquare& other);

protected:
	virtual void resizeEvent(QResizeEvent* event);

private:
	void draw();

	square::type m_type;

signals:

public slots:
};

#endif // GAMESQUARE_HPP
