procedure main is
begin
end main;

function sombra_dupla return Integer is
  x : Integer;
  y : Integer;
  x_interno : Integer;
  y_interno : Integer;
begin
  x := 1;
  y := 2;
  if x > 0 then
    x_interno := 10;
    if x_interno > 5 then
      y_interno := 20;
      return x_interno + y_interno;
    end if;
  end if;
  return x + y;
end sombra_dupla;