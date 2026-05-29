procedure main is
begin
end main;

function sombra_externo return Integer is
  x : Integer;
  x_interno : Integer;
begin
  x := 5;
  if x > 0 then
    x_interno := 10;
  end if;
  return x;
end sombra_externo;