procedure main is
begin
end main;

function calcular_taxa (tipo : Integer) return Float is
begin
  if estaAtivo(tipo) then
    if obterDesconto > 50 then
      return aplicarBonus(total + 10);
    end if;
  end if;
  return 0.0;
end calcular_taxa;
