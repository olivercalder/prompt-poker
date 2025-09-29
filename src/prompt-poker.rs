use std::env;
use std::fs;
use std::io;
use std::io::Write;
use std::os::unix::fs::{OpenOptionsExt, PermissionsExt};
use std::process;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        eprintln!("USAGE: {} FILEPATH", args[0]);
        process::exit(2);
    }
    if let Err(err) = create_write_chmod_file(&args[1]) {
        eprintln!("{}", err);
        process::exit(1);
    }
}

fn create_write_chmod_file(filepath: &str) -> io::Result<()> {
    // create
    let mut file = fs::File::options()
        .create(true)
        .write(true)
        .mode(0o600)
        .open(filepath)?;
    // write
    file.write_all(b"hello prompting\n")?;
    // chmod
    let mut perms = file.metadata()?.permissions();
    perms.set_mode(0o644);
    file.set_permissions(perms)?;
    // sync
    file.sync_all()?;
    // write again
    file.write_all(b"hello again\n")?;
    Ok(())
}
