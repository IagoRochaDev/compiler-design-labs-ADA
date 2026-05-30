procedure main is
begin
end main;

function erroRetornoFloatEmBool(taxa : Float; valor : Integer) return Boolean is
  desconto : Float;
begin
  desconto := valor * taxa;
  return desconto;
end erroRetornoFloatEmBool;
