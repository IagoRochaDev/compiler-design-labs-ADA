procedure main is
begin
end main;

function sombra_tripla return Integer is
  n : Integer;
  n_1 : Integer;
  n_2 : Integer;
  n_3 : Integer;
begin
  n := 1;
  if n > 0 then
    n_1 := 2;
    if n_1 > 0 then
      n_2 := 3;
      if n_2 > 0 then
        n_3 := 4;
        return n_3;
      end if;
    end if;
  end if;
  return n;
end sombra_tripla;