procedure main is
begin
end main;

function verificar_usuario (id : Integer) return Boolean is
  resultado : Boolean := False;
begin
  if validarUsuario(id) = True then
    resultado := True;
    return resultado;
  end if;
  return resultado;
end verificar_usuario;
