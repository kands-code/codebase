pub fn add(x: i32, y: i32) -> i32 {
    x + y
}

pub fn two_sum(num: Vec<i32>, target: i32) -> Vec<(i32, i32)> {
    let mut result_vec = Vec::new();
    let mut num_enum_list = num
        .iter()
        .enumerate()
        .map(|(i, v)| (v, i))
        .collect::<Vec<_>>();
    num_enum_list.sort_by(|v1, v2| v1.0.partial_cmp(v2.0).unwrap());
    let mut low_boundary = 0;
    let mut high_boundary = num_enum_list.len() - 1;
    while low_boundary < high_boundary {
        if let (Some(n1), Some(n2)) = (
            num_enum_list.get(low_boundary),
            num_enum_list.get(high_boundary),
        ) {
            let try_sum = n1.0 + n2.0;
            if try_sum == target {
                result_vec.push((n1.1 as i32, n2.1 as i32));
                high_boundary -= 1;
                low_boundary += 1;
            } else if try_sum > target {
                high_boundary -= 1;
            } else {
                low_boundary += 1;
            }
        }
    }
    result_vec
}
