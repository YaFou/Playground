open Board

(* X--
   X--
   X-- *)
(* let () = let b = make () in *)
(*     play_move b 0 0 Cross; *)
(*     play_move b 0 1 Cross; *)
(*     play_move b 0 2 Cross; *)
(*     assert (check_end b = Some Cross) *)

(* X--
   -X-
   --X *)
let () =
  let b = make () in
  play_move b 0 0 Cross;
  play_move b 1 1 Cross;
  play_move b 2 2 Cross;
  assert (check_end b = Some Cross)
