procedure main is
begin
end main;

function erroRetornoBool(x : Integer) return Integer is
  flag : Boolean;
begin
  flag := x < 0;
  return flag;
end erroRetornoBool;
