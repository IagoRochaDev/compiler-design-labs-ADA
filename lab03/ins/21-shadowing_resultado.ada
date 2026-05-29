procedure main is
begin
end main;

function sombra return Integer is
  resultado : Integer;
  resultado_interno : Integer;
begin
  resultado := 10;
  if resultado > 5 then
    resultado_interno := 20;
    return resultado_interno;
  end if;
  return resultado;
end sombra;