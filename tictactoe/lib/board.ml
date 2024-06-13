type player = Cross | Circle
type board = player option array array

let player_to_char p =
  match p with Some Cross -> 'X' | Some Circle -> 'O' | None -> ' '

let make () = Array.make_matrix 3 3 None
let play_move b (i, j) p = b.(i).(j) <- Some p

let check_end b =
  let row i j = if j = 2 then None else Some (i, j + 1) in
  let column i j = if i = 2 then None else Some (i + 1, j) in
  let diagonal1 i _ = if i = 2 then None else Some (i + 1, i + 1) in
  let diagonal2 i _ = if i = 2 then None else Some (i + 1, i - 1) in
  let rec check_line next_position (i, j) candidate =
    match (b.(i).(j), candidate) with
    | Some p, None -> (
        match next_position i j with
        | None -> None
        | Some pos -> check_line next_position pos (Some p))
    | Some p, Some p' when p = p' -> (
        match next_position i j with
        | None -> Some p
        | Some pos -> check_line next_position pos (Some p))
    | _ -> None
  in
  let exception Game_end of player in
  let check_end next_position pos =
    match check_line next_position pos None with
    | Some p -> raise (Game_end p)
    | _ -> ()
  in
  try
    for i = 0 to 2 do
      check_end row (i, 0);
      check_end column (0, i)
    done;
    check_end diagonal1 (0, 0);
    check_end diagonal2 (0, 2);
    None
  with Game_end p -> Some p

let print b =
  let print_border_line () = print_endline "+---+---+---+" in
  let print_line i =
    for j = 0 to 2 do
      Printf.printf "+ %c " (player_to_char b.(i).(j))
    done;
    print_endline "+"
  in
  for i = 0 to 2 do
    print_border_line ();
    print_line i
  done;
  print_border_line ()

let next_player p = match p with Cross -> Circle | Circle -> Cross
let can_play b (i, j) = match b.(i).(j) with None -> true | _ -> false
