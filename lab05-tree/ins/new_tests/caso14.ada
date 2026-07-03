procedure main is
begin
end main;

function motorDeBusca (chave : Integer) return Integer is
  chaveBusca : Integer;
begin
  chaveBusca := chave;
  while iterarDados loop
    if comparar(chaveBusca, obterChaveAtual) = 0 then
      while extrairMetadados(chaveBusca) loop
        chaveBusca := incrementarChave(chaveBusca, calcularSalto(chaveBusca));
      end loop;
      return formatarResultado(chaveBusca);
    end if;
  end loop;
  return -1;
end motorDeBusca;
