OUTPUT_ARCH( "riscv" )
ENTRY( _entry )

SECTIONS
{

  . = 0x80000000;


  PROVIDE(end = .);
}
