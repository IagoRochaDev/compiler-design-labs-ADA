procedure main is
begin
end main;

function checarSensor (leitura : Float) return Integer is
  copiaLeitura : Float;
begin
  copiaLeitura := leitura;
  if calibrar(copiaLeitura) = obterAlvo then
    if dispararAlarme then
      return 1;
    end if;
  end if;
  if not estaPronto < limiteCritico then
    copiaLeitura := lerSegundaOpcao(copiaLeitura * ajustar);
  end if;
  return verificarSistema;
end checarSensor;
