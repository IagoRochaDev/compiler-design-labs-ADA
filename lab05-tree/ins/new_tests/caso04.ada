procedure main is
begin
end main;

function processar_valor (valor : Float) return Float is
  res : Integer;
begin
  res := calcular(gerarSemente(valor), 42);
  return Float(res);
end processar_valor;
