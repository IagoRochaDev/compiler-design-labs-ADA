procedure main is
begin
end main;

function triplo_aninhado return Integer is
  a : Integer;
  b : Integer;
  c : Integer;
  d : Integer;
begin
  a := 1;
  if a > 0 then
    b := a + 2;
    if b > 0 then
      c := b * 3;
      if c > 0 then
        d := c - 1;
        return d;
      end if;
    end if;
  end if;
  return a;
end triplo_aninhado;