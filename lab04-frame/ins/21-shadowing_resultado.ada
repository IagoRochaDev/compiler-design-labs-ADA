procedure main is
begin
end main;

function sombra return Integer is
  resultado : Integer;
begin
  resultado := 10;
  if resultado > 5 then
    declare
      resultado : Integer;
    begin
      resultado := 20;
      return resultado;
    end;
  end if;
  return resultado;
end sombra;
