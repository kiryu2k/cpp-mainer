unit mystring;

interface

function mylen(const x: string) : longInt;

implementation

function mylen(const x: string) : longInt; alias: 'mylen';
begin
	mylen := length(x);
end;

end.
