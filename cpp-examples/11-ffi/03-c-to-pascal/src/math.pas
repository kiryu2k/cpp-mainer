unit math;

interface

function add(a, b: longInt) : longInt;

implementation

function add(a, b: longInt) : longInt; alias: 'math_add';
begin
	add := a + b;
end;

end.

