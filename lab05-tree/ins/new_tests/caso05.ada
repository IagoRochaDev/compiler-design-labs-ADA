procedure main is
begin
end main;

function contador_loop return Integer is
  contador : Integer := 0;
begin
  while continuarExecucao(contador) loop
    contador := contador + 1;
  end loop;
  return contador;
end contador_loop;
