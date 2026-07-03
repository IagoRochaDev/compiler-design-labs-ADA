procedure main is
begin
end main;

function validarFluxo (status : Integer) return Boolean is
  estadoAtual : Integer;
begin
  estadoAtual := status;
  while verificarStatus(estadoAtual) loop
    if not validarToken(gerarToken) then
      estadoAtual := atualizar(estadoAtual, falha);
    else
      return proximaEtapa;
    end if;
  end loop;
  return False;
end validarFluxo;
