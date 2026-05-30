procedure main is
begin
end main;

function sombra_tripla return Integer is
  n : Integer;
begin
  n := 1;
  if n > 0 then
    declare
      n : Integer;
    begin
      n := 2;
      if n > 0 then
        declare
          n : Integer;
        begin
          n := 3;
          if n > 0 then
            declare
              n : Integer;
            begin
              n := 4;
              return n;
            end;
          end if;
        end;
      end if;
    end;
  end if;
  return n;
end sombra_tripla;
