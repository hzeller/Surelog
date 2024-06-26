package pack;

typedef struct {
   string spath;
} uvm_hdl_path_slice;

class uvm_hdl_path_concat;
   uvm_hdl_path_slice slices[];
endclass

class uvm_mem_mam;

protected virtual function void check_reg();
  uvm_hdl_path_concat paths[$];

  foreach(paths[p]) begin
    uvm_hdl_path_concat path=paths[p];
    foreach (path.slices[j]) begin
      string p_ = path.slices[j].spath; 
    end
  end

endfunction

endclass 

endpackage

