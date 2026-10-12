#!/usr/bin/env rust-script
//! ```cargo
//! [dependencies]
//! common = { path = "../common" }
//! ```

use std::io::{self, Write};
use common::input;

fn main() {
    print!("Enter number: ");
    io::stdout().flush().unwrap();

    let num: i32 = input();

    println!("The number: {num}");
}
