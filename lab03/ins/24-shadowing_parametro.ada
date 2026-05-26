procedure main is
begin
end main;

function sombra_param (n : Integer) return Integer is
begin
  if n > 0 then
    declare
      n : Integer;
    begin
      n := n * 2;
      return n;
    end;
  end if;
  return n;
end sombra_param;
