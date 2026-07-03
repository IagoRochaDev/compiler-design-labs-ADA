procedure main is
begin
end main;

function simular (passos : Integer) return Float is
  contadorPassos : Integer;
begin
  contadorPassos := passos;
  while contadorPassos > obterMinimo loop
    if checarCondicao(contadorPassos + calcularDelta) then
      contadorPassos := contadorPassos - extrairPasso;
    else
      contadorPassos := contadorPassos / aplicarFator(obterDivisor(contadorPassos), retornarInercia);
    end if;
  end loop;
  return obterResultadoFinal;
end simular;
