let b = Board.make ()
let () = print_endline "Play with another player (p) or with the computer (c): "
let against_computer = Scanf.scanf "%c\n" (fun c -> c = 'c')

let rec next_move against_computer p =
  let rec ask_position () =
    let i, j = Scanf.scanf "%d %d\n" (fun i j -> (i - 1, j - 1)) in
    if Board.can_play b (i, j) then (i, j)
    else (
      print_endline "You can't play here";
      ask_position ())
  in
  (match p with
  | Board.Circle when against_computer -> failwith "Not implemented"
  | Circle ->
      print_endline "What is the move for O: ";
      let i, j = ask_position () in
      Board.play_move b (i, j) Circle
  | Cross ->
      print_endline "What is the move for X: ";
      let i, j = ask_position () in
      Board.play_move b (i, j) Cross);
  Board.print b;
  match Board.check_end b with
  | None -> next_move against_computer (Board.next_player p)
  | Some Cross -> print_endline "Player X won"
  | Some Circle -> print_endline "Player O won"

let () = Board.print b
let () = next_move against_computer Cross
