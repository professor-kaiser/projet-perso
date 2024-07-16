<?php session_start(); ?>
<!DOCTYPE HTML>
<HTML>
<HEAD>
	<TITLE>B&lock</TITLE>
	<META charset="UTF8">
	<META name="viewport" content="width=device-width,initial-scale=1.0">
	<LINK rel="stylesheet" href="bootstrap-5.3.3-dist/css/bootstrap.min.css">
	<LINK rel="stylesheet" href="css/style.css">
	<SCRIPT src="bootstrap-5.3.3-dist/js/bootstrap.bundle.min.js" defer></SCRIPT>
	<SCRIPT src="js/front.js" defer></SCRIPT>
</HEAD>
<BODY>
	<DIV class="container-fluid">
		<DIV class="row">
			<DIV class="bg-light d-flex p-2 justify-content-between align-items-center">
				<DIV>
					<H4 class="text-muted mx-3">Sites Jails</H4>
				</DIV>
				<DIV class="d-flex justify-content-between align-items-center">
					<DIV class="me-3">
						<FORM class="d-flex justify-content-between align-items-center" action="power.php" method="POST">
							<DIV class="form-check form-switch">
								<?php $service = (isset($_GET['service']) && $_GET['service'] === 'stop') ? '':'checked'; ?>
			  					<INPUT id="status" onchange="setStatus()" class="form-check-input" type="checkbox" name="status" <?= $service ?>>
							</DIV>
							<BUTTON type="submit" class="btn"><IMG src="src/power.png" width="20"></BUTTON>
						</FORM>
					</DIV>
					<DIV class="dropdown">
						<BUTTON class="btn" type="button" id="menu" data-bs-toggle="dropdown"><IMG src="src/menu.png"></BUTTON>
						<UL class="dropdown-menu" aria-labelledby="menu">
							<LI><A id="open-block" class="dropdown-item">Lock other sites</A></LI>
						</UL>
					</DIV>
				</DIV>
			</DIV>
		</DIV>

		<DIV class="row my-3 mb-5">
			<DIV class="col-md-12">
				<!FORM action="index.php" method="POST">
					<?php $search = isset($_POST['search']) ? trim($_POST['search']):''; ?>
					<DIV class="border p-1 d-flex justify-content-between">
						<INPUT id="search" class="form-control form-control-sm border-0" onkeyup="hello()" name="search" value="<?= $search ?>">
						<BUTTON class="btn" type="submit"><IMG class="mx-2" onclick="hello()" src="src/search.png" width="20"></BUTTON>
					</DIV>
				<!/FORM>
			</DIV>
		</DIV>

		<DIV class="row my-4"> 
			<DIV class="col-md-6 d-flex justify-content-start">
				<?php $status = isset($_SESSION['status']) ? $_SESSION['status']:0; ?>
				<?php $status_label = ($status === 1) ? 'Start':'Stop'; ?>
				<?php $status_color = ($status === 1) ? 'bg-primary':'bg-danger'; ?>
				<DIV><DIV class="badge <?= $status_color ?>"><?= $status_label ?></DIV></DIV>
			</DIV>
		</DIV>

		<DIV class="row fixed-top">
			<?php if(isset($_SESSION['err_msg']) && !empty($_SESSION['err_msg'])) : ?>
			<DIV class="col-md-12">
				<DIV class="alert alert-danger alert-dismissible fade show" role="alert">
					<SPAN><?= $_SESSION['err_msg'] ?></SPAN>
	  				<BUTTON type="button" class="btn-close" data-bs-dismiss="alert" aria-label="Close"></BUTTON>
				</DIV>
			</DIV>
			<?php unset($_SESSION['err_msg']); ?>
			<?php endif; ?>
		</DIV>

		<DIV class="row mb-5">
			<DIV class="col-md-12">
				<TABLE class="table caption-top">
					<CAPTION>List of sites blocked</CAPTION>
					<THEAD>
						<TR class="text-center table-light">	
							<TD>URLs</TD>
							<TD>Date & Time</TD>
							<TD>Action</TD>
						</TR>
					</THEAD>
					<TBODY>
						<?php require_once 'php/Database/Database.php'; ?>
						<?php $db = new Database(); ?>
						<?php $db_result = $db->findAll(); ?>
						<!--?php $db_result = (isset($_POST['search']) && !empty($_POST['search'])) ? $db->find(trim($_POST['search'])):$db->findAll(); ?-->

						<?php foreach($db_result as $data) : ?>
							<?php $color = (checkdnsrr($data['url'], 'A') || checkdnsrr($data['url'], 'AAAA')) ? 'text-primary':'text-secondary'; ?>
							<TR class="text-center">
								<TD class="<?= $color ?>">
									<?php if($db->is_locked($data['url'])) : ?>
										<SPAN class="badge bg-light"><IMG src="src/block.png" width="10"></SPAN>
									<?php endif; ?>
									<SMALL><?= $data['url'] ?></SMALL>
								</TD>
								<TD class="text-muted"><SMALL><?= $data['action_date'] ?></SMALL></TD>
								<TD class="url-blocked">
									<INPUT type="hidden" value="<?= $data['url'] ?>" name="url">
									<INPUT type="hidden" value="<?= $data['password'] ?>" name="password">
									<BUTTON type="button" class="btn btn-sm btn-primary allowButton" disabled>Allow</BUTTON>
								</TD>
							</TR>
						<?php endforeach; ?>
					</TBODY>
				</TABLE>
			</DIV>
		</DIV>
	</DIV>
	<DIV id="blur"></DIV>

	<!-- Ajouter un nouveau site à bloquer -->
	<DIV class="container-fluid">
		<DIV class="row">
			<DIV id="add" class="lock card card-body px-4 pb-4 col-md-6 col-10">
				<DIV class="d-flex justify-content-end">	
					<IMG id="close-block" class="opacity-50 mb-3" src="src/close.png" width="20" height="20">
				</DIV>
				<DIV id="loading-block" class="spinner-border spinner-border-sm text-primary" role="status"></DIV>
				<FORM action="block.php" method="POST">
					<LABEL for="url-to-block" class="form-label">Enter URL</LABEL>
					<INPUT id="url-to-block" class="form-control" name="url" placeholder="www.example.com" required>
					
					<DIV class="form-check form-switch mt-3">
			  			<INPUT id="lock-status" onchange="setSwitch()" class="form-check-input" type="checkbox" name="lock-status">
			  			<LABEL for="lock-status" class="form-label">Lock</LABEL>
					</DIV>
					<INPUT id="set-password" type="password" name="password" class="form-control" placeholder="password">

					<INPUT id="btn-block" type="submit" class="btn btn-danger col-md-12 col-12 mt-5" value="Block">
				</FORM>
			</DIV>
		</DIV>
	</DIV>

	<!-- Formulaire pour débloquer un site -->
	<DIV class="container-fluid">
		<DIV class="row">
			<DIV id="div-allow" class="lock card card-body px-4 pb-4 col-md-6 col-10">
				<FORM id="form-allow" action="allow.php" method="POST">

				</FORM>
			</DIV>
		</DIV>
	</DIV>
</BODY>
</HTML>
