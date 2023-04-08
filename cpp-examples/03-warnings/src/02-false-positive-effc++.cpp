// Пример ложного срабатывания диагностики -Weffc++
//
// Предупреждение gcc (11.2.0):
// warning: ‘Segment::begin_’ should be initialized in the member initialization
// list [-Weffc++]
//   10 |   Segment() {
//      |   ^~~~~~~
// warning: ‘Segment::end_’ should be initialized in the member initialization
// list [-Weffc++]
//
// Поскольку у типа Point есть конструктор по умолчанию, явная инициализация
// полей в Segment не требуется.

struct Point {
  Point() : x_(0), y_(0) {
  }

  double x_;
  double y_;
};

struct Segment {
  Segment() {
  }

  Point begin_;
  Point end_;
};

int main() {
}
