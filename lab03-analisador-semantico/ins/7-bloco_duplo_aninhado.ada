procedure principal is
  x : Integer;
  y : Integer;
begin
  x := 10;
  if x > 5 then
    y := x * 2;
    if y > 10 then
      return y;
    end if;
  end if;
end principal;
