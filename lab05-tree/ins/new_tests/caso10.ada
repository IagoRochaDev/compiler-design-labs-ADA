procedure main is
begin
end main;

function processarMatriz return Integer is
  celula : Integer := 0;
begin
  while possuiLinha loop
    while possuiColuna loop
      celula := processarCelula(obterX, obterY(maximo));
    end loop;
  end loop;
  return celula;
end processarMatriz;
