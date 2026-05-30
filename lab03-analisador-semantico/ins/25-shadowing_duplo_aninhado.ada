procedure main is
begin
end main;

function sombra_dupla return Integer is
  x : Integer;
  y : Integer;
begin
  x := 1;
  y := 2;
  if x > 0 then
    declare
      x : Integer;
    begin
      x := 10;
      if x > 5 then
        declare
          y : Integer;
        begin
          y := 20;
          return x + y;
        end;
      end if;
    end;
  end if;
  return x + y;
end sombra_dupla;
