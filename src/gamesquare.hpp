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

class QMouseEvent;

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

	explicit Gamesquare(QPoint id, QWidget *parent = nullptr);

	void type(square::type new_type);

	square::type type() const;

	void swap(Gamesquare& other);

signals:
	void pressed(int x, int y);
	void released(int x, int y);


protected:
	virtual void mousePressEvent(QMouseEvent* event) override;

	virtual void mouseReleaseEvent(QMouseEvent* event) override;

private:
	void draw();

	square::type m_type;
	const QPoint m_id;
};

#endif // GAMESQUARE_HPP
