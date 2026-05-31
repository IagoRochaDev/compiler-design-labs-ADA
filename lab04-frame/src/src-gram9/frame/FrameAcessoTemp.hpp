#ifndef _FRAME_ACESSO_TEMP_HPP_
#define _FRAME_ACESSO_TEMP_HPP_

class FrameAcessoTemp : public FrameAcesso {
public:
  int id;
  FrameAcessoTemp(int id_registrador) : id(id_registrador) {}
};

#endif
