procedure main is
begin
end main;

function sombra_param (n : Integer) return Integer is
  n_interno : Integer;
begin
  if n > 0 then
    n_interno := n * 2;
    return n_interno;
  end if;
  return n;
end sombra_param;