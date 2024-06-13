type player = Cross | Circle
type board

val make : unit -> board
val play_move : board -> int * int -> player -> unit
val check_end : board -> player option
val print : board -> unit
val next_player : player -> player
val can_play : board -> int * int -> bool
