procedure main is
begin
end main;

function triplo_aninhado return Integer is
  a : Integer;
begin
  a := 1;
  if a > 0 then
    declare
      b : Integer;
    begin
      b := a + 2;
      if b > 0 then
        declare
          c : Integer;
        begin
          c := b * 3;
          if c > 0 then
            declare
              d : Integer;
            begin
              d := c - 1;
              return d;
            end;
          end if;
        end;
      end if;
    end;
  end if;
  return a;
end triplo_aninhado;
