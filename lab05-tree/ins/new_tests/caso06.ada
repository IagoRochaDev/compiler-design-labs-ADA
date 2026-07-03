procedure main is
begin
end main;

function avaliar_x (x : Integer) return Boolean is
  localX : Integer;
begin
  localX := x;
  if checar(localX) < limite() then
    return verificarAtalho(localX);
  else
    localX := localX * recalcular(localX);
  end if;
  return False;
end avaliar_x;
