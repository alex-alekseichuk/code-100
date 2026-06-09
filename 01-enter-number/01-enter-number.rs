#!/usr/bin/env run-cargo-script

use std::io::{self, Write};

fn main() {
    print!("Enter number: ");
    io::stdout().flush().unwrap();

    let mut line = String::new();
    let _ = io::stdin().read_line(&mut line).unwrap();

    let num: i32 = line
        .trim()
        .parse()
        .unwrap();

    println!("The number: {num}");
}
