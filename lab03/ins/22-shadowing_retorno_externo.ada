procedure main is
begin
end main;

function sombra_externo return Integer is
  x : Integer;
begin
  x := 5;
  if x > 0 then
    declare
      x : Integer;
    begin
      x := 10;
    end;
  end if;
  return x;
end sombra_externo;
